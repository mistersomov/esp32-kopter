cmake_minimum_required(VERSION 3.28)
include("$ENV{IDF_PATH}/tools/cmake/project.cmake")
project(esp32_kopter)

idf_build_set_property(COMPILE_OPTIONS "-Wall" APPEND)