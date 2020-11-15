cmake_minimum_required(VERSION 3.5)

project(huffcode LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(huffcode
        IO.cpp
        node.cpp
        input_parsing.cpp
        alphabet.cpp
        file_encoder.cpp
        file_decoder.cpp
        main.cpp
        )
