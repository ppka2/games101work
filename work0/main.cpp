#include<cmath>
#include<Eigen/Core>
#include<Eigen/Dense>
#include<iostream>
#define M_PI 3.14159265358979323846

int main() {

	float x = 2.0f;
	float y = 1.0f;
	float z = 1.0f;
	Eigen::Vector3f v(x, y, z);
	Eigen::Matrix3f tranMatrix;
	Eigen::Matrix3f moveMatrix;
	moveMatrix <<
		1, 0, 1,
		0, 1, 2,
		0, 0, 1;
	auto  angle = 45 * M_PI / 180.0;
	tranMatrix <<
		std::cos(angle), -std::sin(angle), 0,
		std::sin(angle), std::cos(angle), 0,
		0, 0, 1;
	auto result = moveMatrix * tranMatrix * v;
	std::cout << result << std::endl;
	return 0;
}