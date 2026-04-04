cmake_minimum_required(VERSION 3.25)

project(sem2 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(csv_printer src/main.c src/csv.c)
