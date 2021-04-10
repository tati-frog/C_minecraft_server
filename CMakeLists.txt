cmake_minimum_required(VERSION 3.10)

project(McServer C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED true)

file(GLOB_RECURSE SRC_FILES CMAKE_CONFIGURE_DEPENDS "src/*.c")

add_executable(McServer ${SRC_FILES})
target_link_libraries(McServer "pthread" "uuid")
target_include_directories(McServer PUBLIC "src/")