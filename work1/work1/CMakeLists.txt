cmake_minimum_required(VERSION 3.10)
project(Rasterizer)

message("cjjj",${CMAKE_PREFIX_PATH})
find_package(OpenCV REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
set(CMAKE_CXX_STANDARD 17)


add_executable(Rasterizer main.cpp rasterizer.hpp rasterizer.cpp Triangle.hpp Triangle.cpp)
target_link_libraries(Rasterizer ${OpenCV_LIBRARIES})
target_link_libraries(Rasterizer Eigen3::Eigen)