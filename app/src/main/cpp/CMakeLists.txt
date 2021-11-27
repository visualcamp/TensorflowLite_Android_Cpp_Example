cmake_minimum_required(VERSION 3.5)

project(tensorflowlite_android_cpp_example)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-type")

set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party")


add_library(tensorflowlite_android_cpp_example
        SHARED
        native-lib.cpp)


find_library(log-lib log)

add_subdirectory(third_party/opencv)
add_subdirectory(third_party/tflite)


target_link_libraries(tensorflowlite_android_cpp_example
        ${log-lib}
        opencv
        tflite
        )
