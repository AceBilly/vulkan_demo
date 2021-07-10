cmake_minimum_required(VERSION 3.16)
project(vulkan_demo)

set(CMAKE_CXX_STANDARD 20)

aux_source_directory(src/ SRC)

add_executable(vulkan_demo ${SRC})

target_link_libraries(vulkan_demo glfw vulkan)