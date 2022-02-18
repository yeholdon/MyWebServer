cmake_minimum_required(VERSION 3.5)

# Set the project name
project (catch2_unit_test)


set(CMAKE_CXX_STANDARD 11)

# add the CMakeFile that defines catch2
add_subdirectory(3rd_party/catch2)
add_subdirectory(base)
# Add an library for the example classes


#############################################
# Unit tests

# enable CTest testing
enable_testing()

# Add a testing executable
add_executable(unit_tests unit_tests.cpp)
target_include_directories(unit_tests 
    PUBLIC
        ${PROJECT_SOURCE_DIR}    
)
target_link_libraries(unit_tests
    PUBLIC
    sub::liblog
    Catch2::Test
)

add_test(test_all unit_tests)