add_executable(${PROJECT_NAME} main.cpp)

add_library(map map.cpp)
add_library(strategy strategy.cpp)
add_library(kernel kernel.cpp)
add_library(parser parser.cpp)
add_library(utils utils.cpp)
add_library(processor processor.cpp)

target_link_libraries(kernel PRIVATE map strategy)
target_link_libraries(parser PRIVATE kernel map)
target_link_libraries(utils PRIVATE kernel)
target_link_libraries(processor PRIVATE map kernel parser utils)

target_link_libraries(${PROJECT_NAME} PRIVATE processor)
