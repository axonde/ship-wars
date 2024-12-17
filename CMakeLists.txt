cmake_minimum_required(VERSION 3.12)

project(
    ship-wars
    VERSION 0.1
    DESCRIPTION "SHIP-WARS"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

# Поиск Boost в системе
find_package(Boost REQUIRED)

if(Boost_FOUND)
    message(STATUS "Boost found: ${Boost_INCLUDE_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Boost not found!")
endif()


add_subdirectory(src)

# enable_testing()
# add_subdirectory(tests)
