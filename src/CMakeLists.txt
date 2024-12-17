add_executable(${PROJECT_NAME} main.cpp)

add_library(kernel kernel.cpp)
add_library(processor processor.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE kernel)
target_link_libraries(${PROJECT_NAME} PRIVATE processor)