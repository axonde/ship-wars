add_executable(${PROJECT_NAME} main.cpp)

add_library(kernel kernel.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE kernel)