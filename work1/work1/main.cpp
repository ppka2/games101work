#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_rotation(Vector3f axis, float rotation_angle) {
	float angle = rotation_angle / 180 * MY_PI;
	Eigen::Matrix3f N = Eigen::Matrix3f::Identity();
	N <<
		0, -axis.z(), axis.y(),
		axis.z(), 0, -axis.x(),
		-axis.y(), axis.x(), 0;
	Eigen::Matrix3f rod = std::cos(angle) * Eigen::Matrix3f::Identity() + (1 - std::cos(angle)) * axis * axis.transpose() + std::sin(angle) * N;
	Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

	model.block<3, 3>(0, 0) = rod;

	return model;
}


Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
	Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

	Eigen::Matrix4f translate;
	translate <<
		1, 0, 0, -eye_pos[0],
		0, 1, 0, -eye_pos[1],
		0, 0, 1, -eye_pos[2],
		0, 0, 0, 1;

	view = translate * view;

	return view;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
	Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

	auto angle = rotation_angle / 180 * MY_PI;

	Eigen::Matrix2f smallmodel;
	smallmodel <<
		std::cos(angle), -std::sin(angle),
		std::sin(angle), std::cos(angle);
	model.block<2, 2>(0, 0) = smallmodel;
	return model;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
	float zNear, float zFar)
{
	Eigen::Matrix4f perspective;
	perspective <<
		zNear, 0, 0, 0,
		0, zNear, 0, 0,
		0, 0, zNear + zFar, -zNear * zFar,
		0, 0, 1, 0;
	Eigen::Matrix4f orthography;
	float w, h;
	// std::cout << zNear << std::endl;
	h = std::tan(eye_fov / 2) * zNear;
	w = h * aspect_ratio;
	orthography <<
		1 / w, 0, 0, 0,
		0, 1 / h, 0, 0,
		0, 0, 2 / (zFar - zNear), -(zNear + zFar) / 2,
		0, 0, 0, 1;
	Eigen::Matrix4f projection = orthography * perspective;
	return projection;
}

int main(int argc, const char** argv)
{
	float angle = 0;
	bool command_line = false;
	std::string filename = "output.png";

	if (argc >= 3) {
		command_line = true;
		angle = std::stof(argv[2]); // -r by default
		if (argc == 4) {
			filename = std::string(argv[3]);
		}
		else
			return 0;
	}

	rst::rasterizer r(700, 700);

	Eigen::Vector3f eye_pos = { 0, 0, 5 };

	std::vector<Eigen::Vector3f> pos{ {2, 0, -2}, {0, 2, -2}, {-2, 0, -2} };

	std::vector<Eigen::Vector3i> ind{ {0, 1, 2} };

	auto pos_id = r.load_positions(pos);
	auto ind_id = r.load_indices(ind);

	int key = 0;
	int frame_count = 0;

	if (command_line) {
		r.clear(rst::Buffers::Color | rst::Buffers::Depth);

		r.set_model(get_model_matrix(angle));
		r.set_view(get_view_matrix(eye_pos));
		r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

		r.draw(pos_id, ind_id, rst::Primitive::Triangle);
		cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
		image.convertTo(image, CV_8UC3, 1.0f);

		cv::imwrite(filename, image);

		return 0;
	}

	while (key != 27) {
		r.clear(rst::Buffers::Color | rst::Buffers::Depth);

		r.set_model(get_model_matrix(angle));
		r.set_view(get_view_matrix(eye_pos));
		r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

		r.draw(pos_id, ind_id, rst::Primitive::Triangle);

		cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
		image.convertTo(image, CV_8UC3, 1.0f);
		cv::imshow("image", image);
		key = cv::waitKey(10);

		std::cout << "frame count: " << frame_count++ << '\n';

		if (key == 'a') {
			angle += 10;
		}
		else if (key == 'd') {
			angle -= 10;
		}
	}

	return 0;
}
