cmake_minimum_required(VERSION 3.3)
project(OS_labs)

find_package(Threads)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(OS_lab1 lab1/main.cpp)
target_link_libraries(OS_lab1 ${CMAKE_THREAD_LIBS_INIT})

add_executable(OS_lab2 lab2/main.cpp)
target_link_libraries(OS_lab2 ${CMAKE_THREAD_LIBS_INIT})

add_executable(OS_lab3 lab3/main.cpp)
target_link_libraries(OS_lab3 ${CMAKE_THREAD_LIBS_INIT})

add_executable(OS_lab4 lab4/main.cpp)
target_link_libraries(OS_lab4 ${CMAKE_THREAD_LIBS_INIT})

add_executable(OS_lab5 lab5/main.cpp)
target_link_libraries(OS_lab5 ${CMAKE_THREAD_LIBS_INIT})