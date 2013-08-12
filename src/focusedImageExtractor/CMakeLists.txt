find_package( OpenCV REQUIRED )
SET(CMAKE_CXX_FLAGS "-std=c++0x")
add_executable(focusedExtractor focusedExtractor.cpp )
target_link_libraries( focusedExtractor ${OpenCV_LIBS} )

