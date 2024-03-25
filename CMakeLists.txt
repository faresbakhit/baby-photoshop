cmake_minimum_required(VERSION 3.27)
project(baby_photoshop)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(baby_photoshop
        Main.cpp
        stb_image.h
        stb_image_write.h)
