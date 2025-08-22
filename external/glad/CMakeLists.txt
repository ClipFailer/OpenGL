cmake_minimum_required(VERSION 4.1 FATAL_ERROR)
project(glad)

add_library(glad STATIC include/glad/gl.h src/gl.c)
target_include_directories(glad PUBLIC include/)