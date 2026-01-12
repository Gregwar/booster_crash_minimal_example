cmake_minimum_required(VERSION 3.16.3)
project(minimal_example)

add_executable(main main.cpp)
target_link_libraries(main booster_robotics_sdk fastcdr fastrtps)

