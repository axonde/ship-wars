cmake_minimum_required(VERSION 3.12)

project(
    ship-wars
    VERSION 0.1
    DESCRIPTION "SHIP-WARS"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

set(LOCAL_BOOST_PATH "${CMAKE_SOURCE_DIR}/src/boost")
include_directories(SYSTEM ${LOCAL_BOOST_PATH})
message(STATUS "Using local Boost library: ${LOCAL_BOOST_PATH}")

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

add_subdirectory(src)

# enable_testing()
# add_subdirectory(tests)
