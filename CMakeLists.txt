cmake_minimum_required(VERSION 3.20)
project(ProgrammingTasks LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_compile_options(-Wall -Wextra -Wpedantic)

option(ENABLE_BENCHMARKS "Enable benchmarks (requires Google Benchmark)" OFF)
option(ENABLE_TESTS "Enable tests (requires Google Test)" OFF)

set(Boost_USE_STATIC_LIBS    ON)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(Boost 1.85 REQUIRED COMPONENTS 
    filesystem      
    system        
)

add_executable(07_01 07_01.cpp)

add_executable(07_02 07_02.cpp)

add_executable(06_05 06_05.cpp)
target_include_directories(06_05 PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(06_05 PRIVATE ${Boost_LIBRARIES})

add_library(first SHARED first.cpp)
target_include_directories(first PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(first PRIVATE ${Boost_LIBRARIES})

add_library(second SHARED second.cpp)
target_include_directories(second PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(second PRIVATE ${Boost_LIBRARIES})

add_executable(09_07 09_07.cpp)
target_include_directories(09_07 PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(09_07 PRIVATE ${Boost_LIBRARIES})

add_executable(09_08 09_08.cpp)

add_executable(09_09 09_09.cpp)
target_include_directories(09_09 PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(09_09 PRIVATE ${Boost_LIBRARIES} benchmark::benchmark)

add_executable(09_10 09_10.cpp)
target_include_directories(09_10 PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(09_10 PRIVATE ${Boost_LIBRARIES} benchmark::benchmark)

add_executable(10_01 10_01.cpp)

add_executable(10_02 10_02.cpp)

add_executable(10_03 10_03.cpp)
target_include_directories(10_03 PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(10_03 PRIVATE ${Boost_LIBRARIES})

add_executable(10_04 10_04.cpp)
target_include_directories(10_04 PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(10_04 PRIVATE ${Boost_LIBRARIES})

add_executable(11_01 11_01.cpp)

add_executable(11_03 11_03.cpp)

add_executable(11_04 11_04.cpp)

add_executable(11_05 11_05.cpp)

add_executable(11_06 11_06.cpp)
target_include_directories(11_06 PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(11_06 PRIVATE ${Boost_LIBRARIES})

add_executable(12_01 12_01.cpp)

add_executable(12_02 12_02.cpp)

add_executable(12_03 12_03.cpp)

add_executable(12_04 12_04.cpp)

add_executable(12_05 12_05.cpp)

add_executable(13_01 13_01.cpp)

add_executable(13_02 13_02.cpp)

add_executable(13_03 13_03.cpp)

add_executable(13_04 13_04.cpp)

if(ENABLE_TESTS)
    find_package(GTest REQUIRED)
    
    add_executable(07_04 07_04.cpp)
    target_link_libraries(07_04 GTest::gtest GTest::gtest_main pthread)
    target_include_directories(07_04 PRIVATE ${GTEST_INCLUDE_DIRS})
endif()

if(ENABLE_BENCHMARKS)
    find_package(benchmark REQUIRED)
    
    add_executable(07_05 07_05.cpp)
    target_link_libraries(07_05 benchmark::benchmark benchmark::benchmark_main)
endif()

add_executable(08_01 08_01.cpp)

add_executable(08_02 08_02.cpp)

add_executable(08_03 08_03.cpp)

add_executable(08_04 08_04.cpp)

add_executable(08_05 08_05.cpp)

add_executable(09_01 09_01.cpp)

add_executable(09_02 09_02.cpp)

add_executable(09_03 09_03.cpp)

add_executable(09_04 09_04.cpp)

add_executable(09_05 09_05.cpp)

add_executable(09_06 09_06.cpp)
target_include_directories(09_06 PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(09_06 PRIVATE ${Boost_LIBRARIES})

message(STATUS "========================================")
message(STATUS "Project configured with C++${CMAKE_CXX_STANDARD}")
message(STATUS "Build targets:")

get_property(targets DIRECTORY PROPERTY BUILDSYSTEM_TARGETS)
foreach(target ${targets})
    get_target_property(target_type ${target} TYPE)
    if(target_type STREQUAL "EXECUTABLE")
        message(STATUS "  Executable: ${target}")
    elseif(target_type STREQUAL "SHARED_LIBRARY")
        message(STATUS "  Shared library: ${target}")
    endif()
endforeach()

message(STATUS "========================================")
message(STATUS "Build options:")
message(STATUS "  ENABLE_TESTS: ${ENABLE_TESTS} (requires Google Test)")
message(STATUS "  ENABLE_BENCHMARKS: ${ENABLE_BENCHMARKS} (requires Google Benchmark)")
message(STATUS "========================================")