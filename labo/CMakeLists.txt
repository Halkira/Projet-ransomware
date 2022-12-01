cmake_minimum_required(VERSION 3.23)
project(labo C)

set(CMAKE_C_STANDARD 11)

find_package(OpenSSL REQUIRED)

add_executable(labo main.c)
target_link_libraries(labo OpenSSL::SSL)