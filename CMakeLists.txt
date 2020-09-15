cmake_minimum_required(VERSION 3.15)
project(tau_swp_fnl C)

set(CMAKE_C_STANDARD 90)

add_executable(tau_swp_fnl main )

target_link_libraries(tau_swp_fnl m)