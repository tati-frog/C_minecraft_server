cmake_minimum_required(VERSION 3.10)

project(McServer C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED true)

file(GLOB_RECURSE SRC_FILES CMAKE_CONFIGURE_DEPENDS "src/*.c")
file(GLOB_RECURSE TEST_FILES CMAKE_CONFIGURE_DEPENDS "test/*.c" "src/*.c")
list(REMOVE_ITEM TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c")

add_executable(McServer ${SRC_FILES})
add_executable(McServerTest ${TEST_FILES})
target_link_libraries(McServer "pthread" "uuid")
target_link_libraries(McServerTest "uuid" "pthread" "criterion")
target_include_directories(McServer PUBLIC "src/")
target_include_directories(McServerTest PUBLIC "src/")

