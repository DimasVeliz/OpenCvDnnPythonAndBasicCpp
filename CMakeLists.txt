cmake_minimum_required(VERSION 2.8)
project( basicOpenCVFunc )

add_subdirectory(darknetUtility)

add_executable( basicOpenCVFunc main.cpp )
target_link_libraries( basicOpenCVFunc darknetUtility)