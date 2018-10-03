cmake_minimum_required(VERSION 3.0.2)

include(ExternalProject)

set(CMAKE_CXX_STANDARD 17)

project(chibi_bindings)
add_library(chibi_bindings
  include/sexp.hpp
  include/Chibi.hpp

  src/SExp.cpp
  src/Chibi.cpp
  src/RecordTypeDescriptor.cpp
)
target_include_directories(chibi_bindings
  PUBLIC include
  PUBLIC src
)

# Build and get files from chibi

set(CHIBI_PATH ${CMAKE_CURRENT_SOURCE_DIR}/deps/chibi-scheme)
add_custom_target(chibi-scheme
  COMMAND make libchibi-scheme${CMAKE_STATIC_LIBRARY_SUFFIX} lib/chibi.img CHIBI_MODULE_PATH=${CHIBI_PATH}/lib
  WORKING_DIRECTORY ${CHIBI_PATH}
)
add_custom_command(
  TARGET chibi_bindings PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CHIBI_PATH}/libchibi-scheme.so
  ${CMAKE_CURRENT_BINARY_DIR} # Copy the library to link with it
)
add_library(libchibi-scheme STATIC IMPORTED)
set_property(TARGET libchibi-scheme PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/libchibi-scheme.so)

add_custom_command(
  TARGET chibi_bindings PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CHIBI_PATH}/lib
  ${CMAKE_BINARY_DIR}/chibi_lib # Copy runtime libraries to the output directory
)

add_dependencies(chibi_bindings chibi-scheme)
if (UNIX OR APPLE)
  target_link_libraries(chibi_bindings dl)
endif()

target_link_libraries(chibi_bindings libchibi-scheme)
target_include_directories(chibi_bindings
  PUBLIC deps/chibi-scheme/include
)

# Testing

include(CTest)
include(GoogleTest)
find_package(GTest)
add_executable(chibi_bindings_tests
  test/ChibiTests.cpp
  test/SExpTests.cpp
  test/RTDTests.cpp
)
target_link_libraries(chibi_bindings_tests ${GTEST_BOTH_LIBRARIES} chibi_bindings)
gtest_discover_tests(chibi_bindings_tests)
