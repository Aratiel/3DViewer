# note: use "cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++ ." to generate debug version for coverage
#############################################################
#                                                           #
#                   GLOBAL CMAKE SETUP                      #
#                                                           #
#############################################################
option(ENABLE_CODE_ANALYSIS "Run code cppcheck" ON)
cmake_minimum_required(VERSION 3.13)
# disable extentions for pure C++
if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()
enable_testing()
enable_language(CXX)
if(ENABLE_CODE_ANALYSIS)
    find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
    if (CMAKE_CXX_CPPCHECK)
        list(
            APPEND CMAKE_CXX_CPPCHECK 
                "--enable=warning"
                "--inconclusive"
                "--force" 
                "--inline-suppr"
                "--suppressions-list=${CMAKE_SOURCE_DIR}/CppCheckSuppressions.txt"
        )
    endif()
endif(ENABLE_CODE_ANALYSIS)

#############################################################
#                                                           #
#                         LIBRARY                           #
#                                                           #
#############################################################
project(s21_matrix_oop VERSION 1.0 LANGUAGES CXX)
include (CTest)
set(CMAKE_CXX_FLAGS "-Werror -Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wenum-compare -Wfloat-equal -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wsign-promo ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-Wno-deprecated-register -O0 -g -Werror -Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wenum-compare -Wfloat-equal -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wsign-promo ${CMAKE_CXX_FLAGS_DEBUG}")
set(SRC matrix.cpp)
add_library(s21_matrix_oop STATIC ${SRC})
set_property(TARGET s21_matrix_oop PROPERTY CXX_STANDARD 17)
set_property(TARGET s21_matrix_oop PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)
target_include_directories(s21_matrix_oop PUBLIC
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
)
set_property(
    TARGET s21_matrix_oop
    APPEND
    PROPERTY ADDITIONAL_CLEAN_FILES install_manifest.txt coverage
)

#############################################################
#                                                           #
#                         TESTING                           #
#                                                           #
#############################################################
if (GTEST_FOUND)
    include(CTest)
    set(CMAKE_CXX_FLAGS "-Werror -Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wenum-compare -Wfloat-equal -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wsign-promo -g ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "-Wno-deprecated-register -O0 -g -Werror -Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wenum-compare -Wfloat-equal -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wsign-promo ${CMAKE_CXX_FLAGS_DEBUG}")
    set(TEST_SRC tests/tests.cpp)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    add_executable(unit_tests_matrix ${TEST_SRC})
    set_property(TARGET unit_tests_matrix PROPERTY CXX_STANDARD 17)
    set_property(TARGET unit_tests_matrix PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)
    
    set_property(TARGET unit_tests_matrix PROPERTY THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(unit_tests_matrix
        gtest
        gtest_main
        s21_matrix_oop
        Threads::Threads
    )

    set_property(
        TARGET unit_tests_matrix
        APPEND
        PROPERTY ADDITIONAL_CLEAN_FILES Temporary Testing TAG
    )
    add_test(MATRIX_UNIT_TESTS unit_tests_matrix)
endif()

#############################################################
#                                                           #
#                        COVERAGE                           #
#                                                           #
#############################################################

if (GTEST_FOUND)
   find_program(VALGRIND NAMES valgrind)
   if (VALGRIND)
       add_custom_target(memcheck_matrix
           COMMAND ${VALGRIND} -s --trace-children=yes --track-origins=yes --leak-check=yes --quiet ./unit_tests_matrix
           WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
       add_dependencies(memcheck_matrix unit_tests_matrix)
   endif()

   # Proper way but there are still reachable blocks
   # add_custom_target(memcheck
   #     COMMAND ${CMAKE_CTEST_COMMAND}
   #         --force-new-ctest-process --test-action memcheck
   #     COMMAND cat "${CMAKE_BINARY_DIR}/Testing/Temporary/MemoryChecker.*.log"
   #     WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")

endif()
