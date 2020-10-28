cmake_minimum_required(VERSION 2.8)
project( basicOpenCVFunc )
find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( basicOpenCVFunc main.cpp )
target_link_libraries( basicOpenCVFunc ${OpenCV_LIBS} )