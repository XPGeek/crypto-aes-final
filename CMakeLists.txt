cmake_minimum_required(VERSION 3.5)
project(CRYPTO CXX)

# add any source files you create to this variable
set(SOURCE_FILES
        "crypto.hpp" "crypto.cpp"
        "encrypt.hpp" "encrypt.cpp"
        "decrypt.hpp" "decrypt.cpp")

# setup all the testing bs
set(TEST_FILE_DIR "${CMAKE_SOURCE_DIR}/tests")
configure_file(${CMAKE_SOURCE_DIR}/test_config.hpp.in ${CMAKE_BINARY_DIR}/test_config.hpp)

# try to prevent accidental in-source builds, these cause lots of problems
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Remove any files created thus far and use a different directory for the build.")
endif()

# require a C++11 compiler and use it for all targets
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# configure Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5 COMPONENTS Multimedia Widgets Test REQUIRED)

add_executable(crypto main.cpp ${SOURCE_FILES})
target_link_libraries(crypto Qt5::Widgets)

add_executable(test_crypto test_crypto.cpp ${SOURCE_FILES})
target_link_libraries(test_crypto Qt5::Widgets Qt5::Test)

# setup testing
enable_testing()
add_test(test_crypto test_crypto)
