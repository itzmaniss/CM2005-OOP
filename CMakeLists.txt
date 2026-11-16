cmake_minimum_required(VERSION 4.2)
project(OOP)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(OOP
        src/data.cpp
        src/data.h
        src/main.cpp
        src/MerkleMain.cpp
        src/MerkleMain.h
        src/OrderBookEntry.cpp
        src/OrderBookEntry.h)
