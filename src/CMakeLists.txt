cmake_minimum_required(VERSION 3.5)

project(huffcode LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(huffcode
        main.cpp
        node.cpp
        alphabet.cpp
        encode_file.cpp
        bit_level.h
        )
