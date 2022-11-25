cmake_minimum_required(VERSION 3.23)
project(Labo C)

set(CMAKE_C_STANDARD 11)

add_executable(Labo
        main.c
        )

find_package(OpenSSL REQUIRED)
target_link_libraries(untitled OpenSSL::SSL)
