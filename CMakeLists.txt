cmake_minimum_required(VERSION 3.20)
project(ProgrammingTasks LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_compile_options(-Wall -Wextra -Wpedantic)

option(ENABLE_BENCHMARKS "Enable benchmarks (requires Google Benchmark)" OFF)
option(ENABLE_TESTS "Enable tests (requires Google Test)" OFF)

# Настройки для Boost как в примере
set(Boost_USE_STATIC_LIBS    ON)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(Boost 1.85 REQUIRED COMPONENTS 
    filesystem      
    system        
)


#===============================================================================
# Задача 07.01
#===============================================================================
add_executable(07_01_solver 07_01.cpp)
target_compile_options(07_01_solver PRIVATE -Wall -Wextra -Wpedantic)

#===============================================================================
# Задача 07.02 
#===============================================================================
add_executable(07_02_rational 07_02.cpp)
target_compile_options(07_02_rational PRIVATE -Wall -Wextra -Wpedantic)

#===============================================================================
# Задача 06.05
#===============================================================================
# Исполняемый файл
add_executable(06_05_loader 06_05.cpp)
target_compile_options(06_05_loader PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(06_05_loader PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(06_05_loader PRIVATE ${Boost_LIBRARIES})

# Первая библиотека
add_library(first SHARED first.cpp)
target_compile_options(first PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(first PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(first PRIVATE ${Boost_LIBRARIES})

# Вторая библиотека
add_library(second SHARED second.cpp)
target_compile_options(second PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(second PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(second PRIVATE ${Boost_LIBRARIES})

#===============================================================================
# Задача 07.04 
#===============================================================================
if(ENABLE_TESTS)
    find_package(GTest REQUIRED)
    find_package(GMock REQUIRED)
    
    add_executable(07_04_sort_tests 07_04.cpp)
    target_link_libraries(07_04_sort_tests 
        ${GTEST_LIBRARIES}
        ${GMOCK_LIBRARIES}
        pthread
    )
    target_compile_options(07_04_sort_tests PRIVATE -Wall -Wextra -Wpedantic)
    target_include_directories(07_04_sort_tests PRIVATE ${GTEST_INCLUDE_DIRS})
endif()

#===============================================================================
# Задача 07.05 
#===============================================================================
if(ENABLE_BENCHMARKS)
    find_package(benchmark REQUIRED)
    
    add_executable(07_05_sort_benchmarks 07_05.cpp)
    target_link_libraries(07_05_sort_benchmarks benchmark::benchmark benchmark::benchmark_main)
    target_compile_options(07_05_sort_benchmarks PRIVATE -Wall -Wextra -Wpedantic)
endif()

#===============================================================================
# Задача 08.01 
#===============================================================================
add_executable(08_01_fraud 08_01.cpp)
target_compile_options(08_01_fraud PRIVATE -Wall -Wextra -Wpedantic)

#===============================================================================
# Задача 08.02 
#===============================================================================
add_executable(08_02_task 08_02.cpp)
target_compile_options(08_02_task PRIVATE -Wall -Wextra -Wpedantic)

#===============================================================================
# Задача 08.03 
#===============================================================================
add_executable(08_03_ilog2 08_03.cpp)
target_compile_options(08_03_ilog2 PRIVATE -Wall -Wextra -Wpedantic)

#===============================================================================
# Задача 08.04 
#===============================================================================
add_executable(08_04_dawkins 08_04.cpp)
target_compile_options(08_04_dawkins PRIVATE -Wall -Wextra -Wpedantic)

#===============================================================================
# Задача 08.05 
#===============================================================================
add_executable(08_05_timer 08_05.cpp)
target_compile_options(08_05_timer PRIVATE -Wall -Wextra -Wpedantic)

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