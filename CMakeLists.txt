cmake_minimum_required(VERSION 3.8)
project(Image2Lines2)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

set(SOURCE_FILES main.cpp src/Components/Line.cpp src/Components/Line.h src/Components/Valley.cpp src/Components/Valley.h src/Components/Peak.cpp src/Components/Peak.h src/Components/Region.cpp src/Components/Region.h src/LineSegmentation.cpp src/LineSegmentation.h src/Components/Chunk.cpp src/Components/Chunk.h src/Utilities/Utilities.cpp src/Utilities/Utilities.h src/Utilities/Constants.h)
add_executable(Image2Lines2 ${SOURCE_FILES})

target_link_libraries(Image2Lines2 ${OpenCV_LIBS})