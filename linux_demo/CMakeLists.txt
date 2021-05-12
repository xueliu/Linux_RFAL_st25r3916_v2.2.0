# CMakeLists text file of the project

cmake_minimum_required(VERSION 2.8.11)

project(rfal_demo)

# RFAL library variant to link the executable with
set(RFAL_VARIANT "st25r3916" CACHE STRING "Select the RFAL library")

add_definitions(-DST25R3916)

# Compile the RFAL library variant to link the executable with
add_subdirectory(rfal/${RFAL_VARIANT})
add_subdirectory(demo)
