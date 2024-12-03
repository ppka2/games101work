cmake_minimum_required (VERSION 2.8.11)
project (work0)

find_package(Eigen3 CONFIG REQUIRED)

add_executable (work0 main.cpp)

target_link_libraries(work0 PRIVATE Eigen3::Eigen)
