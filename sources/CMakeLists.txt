cmake_minimum_required(VERSION 3.11)

project(
	RECORDER 
	DESCRIPTION "Driver tests."
)

find_package(Doxygen)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# Each option amps up warnings.
add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-pedantic)
add_compile_options(-Werror)

include_directories(
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}
)

add_executable(
    TEST_DEVICE
    test-device.cpp
)
set_target_properties(
    TEST_DEVICE
    PROPERTIES
    RUNTIME_OUTPUT_NAME test-device
)
