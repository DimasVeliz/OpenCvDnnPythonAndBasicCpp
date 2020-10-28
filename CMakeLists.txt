cmake_minimum_required(VERSION 2.8)
project( testingDnn )
find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( testingDnn main.cpp )
target_link_libraries( testingDnn ${OpenCV_LIBS} )