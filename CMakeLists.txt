cmake_minimum_required(VERSION 3.10)
project(stub)

find_package(Python COMPONENTS Interpreter)
# Use waf to resolve dependencies
if(NOT DEFINED STEINWURF_RESOLVE)
  message(STATUS "Resolving dependencies...")
  execute_process(
    COMMAND ${Python_EXECUTABLE} waf resolve ${STEINWURF_RESOLVE_OPTIONS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE STATUS)
  if(STATUS AND NOT STATUS EQUAL 0)
    message(FATAL_ERROR "Failed: ${STATUS}")
  endif()
  set(STEINWURF_RESOLVE "${CMAKE_CURRENT_SOURCE_DIR}/resolve_symlinks")

endif()

# Define library
add_library(stub INTERFACE)
target_compile_features(stub INTERFACE cxx_std_14)
target_include_directories(stub INTERFACE src/)
add_library(steinwurf::stub ALIAS stub)

# Install headers
install(
  DIRECTORY ./src/stub
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include
  FILES_MATCHING
  PATTERN *.hpp)

# Is top level project?
if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})

  # Setup testing
  enable_testing()

  # Google Test dependency
  add_subdirectory("${STEINWURF_RESOLVE}/gtest" EXCLUDE_FROM_ALL)

  # Build test executable
  file(GLOB_RECURSE stub_test_sources ./test/*.cpp)


  add_executable(stub_test ${stub_test_sources})
  target_link_libraries(stub_test stub)
  target_link_libraries(stub_test steinwurf::gtest)

  add_test(NAME stub_test COMMAND stub_test)

endif()
