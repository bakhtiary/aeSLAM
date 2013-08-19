cmake_minimum_required(VERSION 2.8)
project( aeSlam )
#include_directories ("${PROJECT_SOURCE_DIR}/focusedImageExtractor") this indicates that theses folders will be searched for header files.
add_subdirectory ("${PROJECT_SOURCE_DIR}/focusedImageExtractor")
add_subdirectory ("${PROJECT_SOURCE_DIR}/appBuildMap")
add_subdirectory ("${PROJECT_SOURCE_DIR}/libviso2_Opencv")

