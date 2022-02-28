cmake_minimum_required(VERSION 3.14)

if (WIN32)
    project(derivative LANGUAGES CXX)
elseif(UNIX)
    project(derivative)
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(BUILD_SHARED_LIBS OFF)

include(FindDoxygen)
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/docs)
set(DOXYGEN_OUTPUT_LANGUAGE Russian)
set(DOXYGEN_USE_MATHJAX YES)
set(DOCUMENTABLE_SOURCES ${CMAKE_SOURCE_DIR}/src)
doxygen_add_docs(doxygen ${DOCUMENTABLE_SOURCES})

include_directories(src)

add_subdirectory(src)
add_subdirectory(tests)
