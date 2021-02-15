set(GCC_COVERAGE_COMPILE_FLAGS "-fPIC")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )


add_library(
    darknetUtility SHARED
    src/DarknetUtility.hpp
    src/DarknetUtility.cpp
)
target_include_directories(darknetUtility PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src") 
target_link_libraries(darknetUtility ${OpenCV_LIBS})


