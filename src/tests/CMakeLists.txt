# note: use "cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++ ." to generate debug version for coverage
# Copyright <lwolmer> 2022
#############################################################
#                                                           #
#                         TESTING                           #
#                                                           #
#############################################################
if (GTEST_FOUND)
    set(GTestFiles ${GTestSrc}/src/gtest-all.cc)
    set(GTestIncludes ${GTestSrc} ${GTestSrc}/include)
    if (NOT ${GMockSrc} STREQUAL "")
        list(APPEND GTestFiles ${GMockSrc}/src/gmock-all.cc)
        list(APPEND GTestIncludes ${GMockSrc} ${GMockSrc}/include)
    endif ()

    include_directories(${GTestIncludes})


    set(CMAKE_CXX_FLAGS "-Werror -Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wenum-compare -Wfloat-equal -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wsign-promo -g ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -fprofile-arcs -ftest-coverage -Werror -Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wenum-compare -Wfloat-equal -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wsign-promo ${CMAKE_CXX_FLAGS_DEBUG}")
    option(CodeCoverage "CodeCoverage" ON)
    include(CTest)
    file(GLOB TEST_SRC
        "*.cc"
    )
    set(TEST_SRC ${CMAKE_SOURCE_DIR}/models/parser.cc ${CMAKE_SOURCE_DIR}/models/mvpchange.cc "${TEST_SRC}")
    add_executable(unit_tests ${TEST_SRC})
    include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/controllers ${CMAKE_SOURCE_DIR}/models ${CMAKE_SOURCE_DIR}/s21_matrixplus ${CMAKE_SOURCE_DIR}/include ${gtest_SOURCE_DIR}/include ${gmock_SOURCE_DIR}/include)
    set_property(TARGET unit_tests PROPERTY CXX_STANDARD 17)
    set_property(TARGET unit_tests PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)
    set_property(TARGET unit_tests PROPERTY THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(unit_tests
        gtest
        stdc++
        m
        gtest_main
        Threads::Threads
        s21_matrix_oop
    )

    file(GLOB TEST_CLEAN_FILES
        "*.dot"
    )
    set_property(
        TARGET unit_tests
        APPEND
        PROPERTY ADDITIONAL_CLEAN_FILES Temporary Testing TAG ${CMAKE_BINARY_DIR}/coverage ${TEST_CLEAN_FILES}
    )
    add_test(3DVIEWER_UNIT_TESTS unit_tests)
endif()

#############################################################
#                                                           #
#                        COVERAGE                           #
#                                                           #
#############################################################

if (GTEST_FOUND)
   if (CodeCoverage MATCHES ON)
       include(CodeCoverage)
       setup_target_for_coverage(coverage unit_tests coverage)
       set_property(
           TARGET coverage
           APPEND
           PROPERTY ADDITIONAL_CLEAN_FILES DartConfiguration.tcl
       )
   endif()
    
endif()

#############################################################
#                                                           #
#                    additional checks                      #
#                                                           #
#############################################################
if (GTEST_FOUND)
        add_custom_target(memcheck
            COMMAND CK_FORK=no leaks --atExit --  ${CMAKE_CURRENT_BINARY_DIR}/unit_tests
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
        add_dependencies(memcheck unit_tests)

    # Proper way but there are still reachable blocks
    # add_custom_target(memcheck
    #     COMMAND ${CMAKE_CTEST_COMMAND}
    #         --force-new-ctest-process --test-action memcheck
    #     COMMAND cat "${CMAKE_BINARY_DIR}/Testing/Temporary/MemoryChecker.*.log"
    #     WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
endif()
