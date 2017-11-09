cmake_minimum_required(VERSION 3.7)

project(Image2Lines)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/main.cpp src/LineSegmenter.hpp src/LineSegmenter.cpp)

add_executable(Image2Char ${SOURCE_FILES})
target_link_libraries(Image2Char ${OpenCV_LIBS})