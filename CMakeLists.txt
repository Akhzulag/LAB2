cmake_minimum_required(VERSION 3.26)
project(LAB2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Find OpenSSL
find_package(OpenSSL REQUIRED)
set(CMAKE_C_COMPILER /opt/homebrew/opt/llvm/bin/clang)
set(CMAKE_CXX_COMPILER /opt/homebrew/opt/llvm/bin/clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
add_executable(LAB2 main.cpp)

# Link against OpenSSL
target_link_libraries(LAB2 PRIVATE OpenSSL::SSL OpenSSL::Crypto omp)
