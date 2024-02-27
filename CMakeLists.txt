cmake_minimum_required(VERSION 3.0)
project(hpx_example_program)

set(CMAKE_CXX_STANDARD 11)


find_package(HPX)


include_directories(HPX)


add_executable(hpx_example_program matrixmult.cpp)
target_link_libraries(hpx_example_program tbb HPX::hpx)