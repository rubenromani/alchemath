#!/bin/bash

# Test runner script for AlcheMath Engine

echo "======================================"
echo "     AlcheMath Engine Test Suite      "
echo "======================================"

# Create build directory if it doesn't exist
BUILD_DIR="build"
if [ ! -d "$BUILD_DIR" ]; then
    mkdir -p "$BUILD_DIR"
fi

cd "$BUILD_DIR"

# Configure with CMake
echo "Configuring with CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Debug

if [ $? -ne 0 ]; then
    echo "❌ CMake configuration failed"
    exit 1
fi

# Build the tests
echo "Building tests..."
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo "❌ Build failed"
    exit 1
fi

echo "✅ Build successful"

# Run the tests
echo ""
echo "Running tests..."
echo "=================="

# Run with verbose output
./engine_tests --gtest_output=xml:test_results.xml --gtest_color=yes

# Store test result
TEST_RESULT=$?

echo ""
echo "=================="

if [ $TEST_RESULT -eq 0 ]; then
    echo "✅ All tests passed!"
else
    echo "❌ Some tests failed!"
fi

# Display summary
echo ""
echo "Test Results Summary:"
echo "- Test executable: $PWD/engine_tests"
echo "- XML report: $PWD/test_results.xml"
echo "- Return code: $TEST_RESULT"

exit $TEST_RESULT