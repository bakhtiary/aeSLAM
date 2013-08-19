# project
find_package( OpenCV REQUIRED )

# directories
set (LIBVISO2_SRC_DIR libviso2src)

# include directory
include_directories("${LIBVISO2_SRC_DIR}")

# use sse3 instruction set
SET(CMAKE_CXX_FLAGS "-msse3")

# sources
FILE(GLOB LIBVISO2_SRC_FILES "libviso2src/*.cpp")

# demo program
add_executable(libviso2OpenCV ${LIBVISO2_SRC_FILES} libviso2OpenCV.cpp)
target_link_libraries (libviso2OpenCV ${OpenCV_LIBS})

add_executable(libviso2_origDemo ${LIBVISO2_SRC_FILES} demo.cpp)
target_link_libraries (libviso2_origDemo png)



