cmake_minimum_required(VERSION 3.28)
project(OpenCvProject)
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(OpenCvProject main.cpp
        DBSCAN.cpp
        Calculate.cpp
        Color.cpp
        Movement.cpp
        PointID.cpp)
target_link_libraries( OpenCvProject  ${OpenCV_LIBS})
set(CMAKE_CXX_STANDARD 17)

