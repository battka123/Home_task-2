cmake_minimum_required(VERSION 3.23)
project(encryption_decryption)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(demo
        ${CMAKE_CURRENT_SOURCE_DIR}/demo/main.cpp
        )

target_link_libraries(demo ${PROJECT_NAME})



