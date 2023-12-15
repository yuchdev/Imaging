cmake_minimum_required(VERSION 3.12)
project(OpenCVImageStitching)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Set C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)

# Add executable
add_executable(image_stitching main.cpp)

# Link against OpenCV
target_link_libraries(image_stitching ${OpenCV_LIBS})
