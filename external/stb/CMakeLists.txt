cmake_minimum_required(VERSION 3.10)

project(stb)

add_library(stb STATIC include/stb_image.h src/stb_image.cpp)
target_include_directories(stb PUBLIC include/)