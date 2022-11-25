cmake_minimum_required(VERSION 3.23)
project(Labo C)

set(CMAKE_C_STANDARD 11)

find_package(OpenSSL REQUIRED)
add_executable(Labo main.c)
target_link_libraries(labo OpenSSL::SSL)