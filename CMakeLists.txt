cmake_minimum_required(VERSION 3.14) # Because we are using FetchContent which is available in 3.14+
project(DSA)

# # Globbing source files
# file(GLOB_RECURSE SOURCES "includes/**/*.cpp")

# add_library(DSA ${SOURCES})

# # Specify include directories
# target_include_directories(DSA PUBLIC 
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
#     $<INSTALL_INTERFACE:include>  # <prefix>/include
# )


# Adding interface library (header-only)
add_library(DSA INTERFACE)

# Specify include directories
target_include_directories(DSA INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
    $<INSTALL_INTERFACE:include>  # <prefix>/include
)

include(GNUInstallDirs)
if(MSVC)
    add_compile_options(/W4)
    add_link_options(/W4)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})

if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

## Change ON <-> OFF for testing
option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    set(INCLUDES_FOLDER includes)
    FILE(GLOB_RECURSE SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${INCLUDES_FOLDER}/*.cpp")

    ADD_EXECUTABLE(vector_test
        _tests/_test_files/vector_test.cpp
        ${SOURCE_FILES}
    )
    ADD_EXECUTABLE(sll_test
        _tests/_test_files/sll_test.cpp
        ${SOURCE_FILES}
    )
    ADD_EXECUTABLE(stackandqueue_test
        _tests/_test_files/stackandqueue_test.cpp
        ${SOURCE_FILES}
    )
    ADD_EXECUTABLE(avl_test
        _tests/_test_files/avl_test.cpp
        ${SOURCE_FILES}
    )
    ADD_EXECUTABLE(heap_test
        _tests/_test_files/heap_test.cpp
        ${SOURCE_FILES}
    )
    ADD_EXECUTABLE(priorityqueue_test
        _tests/_test_files/priorityqueue_test.cpp
        ${SOURCE_FILES}
    )
    ADD_EXECUTABLE(main
        main.cpp
        ${SOURCE_FILES}
    )
    TARGET_LINK_LIBRARIES(main gtest)
    TARGET_LINK_LIBRARIES(vector_test gtest)
    TARGET_LINK_LIBRARIES(sll_test gtest)
    TARGET_LINK_LIBRARIES(stackandqueue_test gtest)
    TARGET_LINK_LIBRARIES(avl_test gtest)
    TARGET_LINK_LIBRARIES(heap_test gtest)
    TARGET_LINK_LIBRARIES(priorityqueue_test gtest)
endif()

set(CMAKE_CXX_STANDARD 14)

# Install library target
install(TARGETS DSA
    EXPORT DSATargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# This makes project importable from the install directory
# Put config file into the CMake user package registry
install(EXPORT DSATargets
    FILE DSATargets.cmake
    NAMESPACE DSA::
    DESTINATION lib/cmake/DSA
)

# This makes project importable from the build directory
export(TARGETS DSA
    NAMESPACE DSA::
    FILE ${CMAKE_CURRENT_BINARY_DIR}/DSA.cmake
)
