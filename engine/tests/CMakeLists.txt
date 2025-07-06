cmake_minimum_required(VERSION 3.14)
project(AlcheMathEngineTests)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find GoogleTest
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTEST REQUIRED gtest_main)
pkg_check_modules(GMOCK REQUIRED gmock_main)

# Include directories
include_directories(../src/core/DataManager/include)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMOCK_INCLUDE_DIRS})

# Add test executable
add_executable(
  engine_tests
  test_timeseries.cpp
  test_contract.cpp
  test_csv_reader.cpp
  test_data_manager.cpp
  test_main.cpp
  # Add source files that need to be tested
  ../src/core/DataManager/TimeSeries.cpp
  ../src/core/DataManager/ContractCsvReader.cpp
  ../src/core/DataManager/DataManager.cpp
)

# Link libraries
target_link_libraries(
  engine_tests
  ${GTEST_LIBRARIES}
  ${GMOCK_LIBRARIES}
  pthread
)

# Add compiler flags
target_compile_options(engine_tests PRIVATE ${GTEST_CFLAGS_OTHER} ${GMOCK_CFLAGS_OTHER})