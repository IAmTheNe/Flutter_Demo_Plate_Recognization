cmake_minimum_required(VERSION 3.10.2)

#opencv
set(OpenCV_STATIC ON)
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../OpenCV/sdk/native/jni")
find_package(OpenCV REQUIRED)

add_library(
    Native
    SHARED
    # Provides a relative path to your source file(s).
    ../ios/Classes/LicenseDetector/LicenseDetector.cpp
    ../ios/Classes/native_opencv.cpp
)

target_include_directories(
    Native PRIVATE
    ../ios/Classes/LicenseDetector
)

find_library(log-lib log)
target_link_libraries(
    # Specifies the target library
    Native
    ${OpenCV_LIBS}
    ${log-lib}
)