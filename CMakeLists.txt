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

add_executable(OS_lab7 lab7/main.cpp)
target_link_libraries(OS_lab7 ${CMAKE_THREAD_LIBS_INIT})

add_executable(OS_lab10 lab10/main.cpp)
target_link_libraries(OS_lab10 ${CMAKE_THREAD_LIBS_INIT})

add_executable(OS_lab12 lab12/main.cpp)
target_link_libraries(OS_lab12 ${CMAKE_THREAD_LIBS_INIT})

add_executable(OS_lab13 lab13/main.cpp)
target_link_libraries(OS_lab13 ${CMAKE_THREAD_LIBS_INIT})

add_executable(OS_lab15 lab15/main.cpp)
target_link_libraries(OS_lab15 ${CMAKE_THREAD_LIBS_INIT})

add_executable(OS_lab16 lab16/main.cpp lab16/list.cpp)
target_link_libraries(OS_lab16 ${CMAKE_THREAD_LIBS_INIT})

add_executable(OS_lab19 lab19/main.cpp lab19/list.cpp)
target_link_libraries(OS_lab19 ${CMAKE_THREAD_LIBS_INIT})

add_executable(OS_lab22 lab22/main.cpp lab22/myQueue.cpp lab22/detail_type.h)
target_link_libraries(OS_lab22 ${CMAKE_THREAD_LIBS_INIT})

add_executable(OS_lab23 lab23/main.cpp lab23/myQueue.cpp lab23/myQueue.h)
target_link_libraries(OS_lab23 ${CMAKE_THREAD_LIBS_INIT})

add_executable(OS_lab24 lab24/main.cpp lab24/myQueue.cpp lab24/myQueue.h)
target_link_libraries(OS_lab24 ${CMAKE_THREAD_LIBS_INIT})

add_executable(OS_lab25 lab25/main.cpp lab25/Forwarder.cpp lab25/Forwarder.h lab25/tools.cpp lab25/tools.h lab25/Connection.h)
target_link_libraries(OS_lab25 ${CMAKE_THREAD_LIBS_INIT})
