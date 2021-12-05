cmake_minimum_required(VERSION 3.5)

project(seeso_tfl_cpp_native)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-type")

set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party")


add_library(seeso_tfl_cpp_native
        SHARED
        native-lib.cpp
        detector.cc
        )


find_library(log-lib z log)

# Find OpenCV
set( OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/OpenCV-android-sdk/sdk/native/jni" )
find_package(OpenCV REQUIRED)

# Add TFLite
add_subdirectory(third_party/tflite)


target_link_libraries(seeso_tfl_cpp_native
        ${log-lib}
        z
        log
        ${OpenCV_LIBS}
        tflite
        )

target_include_directories(seeso_tfl_cpp_native INTERFACE "${OPENCV_INCLUDE_PATH}")
