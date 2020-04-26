cmake_minimum_required(VERSION 2.4)

# OpenCV Config
SET(OpenCV_DIR "/home/liweijie/opencv-4.3.0/build")
find_package( OpenCV REQUIRED )
set(OpenCV_FOUND 1)
message(STATUS "    version: ${OpenCV_VERSION}")
message("-- OPENCV include:   " ${OpenCV_INCLUDE_DIRS})
message("-- OPENCV libs dir:  " ${OpenCV_LIB_DIR})
message("-- OPENCV libs:   " ${OpenCV_LIBS} )

include_directories(${CMAKE_SOURCE_DIR}/includes)
link_directories(${CMAKE_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
LINK_DIRECTORIES(${OpenCV_LIB_DIR})

add_library(ioUtils "src/ioUtils.cpp")
set(LIBS ${LIBS} ioUtils)

ADD_EXECUTABLE(
      compute_flow
      src/compute_flow.cpp
    )

TARGET_LINK_LIBRARIES(
    compute_flow
    ${OPENCV_LIBRARIES}
    ${OpenCV_LIBS}
    ${LIBS}
)