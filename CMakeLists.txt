cmake_minimum_required(VERSION 3.27)
project(renderglbplus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OSX_ARCHITECTURES "x86_64")

add_executable(renderglbplus main.cpp stb/stb_image_write.h tinygltf/tiny_gltf.cc)

target_link_libraries(renderglbplus glut GL GLU png jpeg m)