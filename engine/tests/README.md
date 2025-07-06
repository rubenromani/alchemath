# AlcheMath Engine Test Suite

This directory contains comprehensive unit tests for the AlcheMath Engine components.

## Test Structure

### Test Files
- `test_timeseries.cpp` - Tests for TimeSeries class functionality
- `test_contract.cpp` - Tests for Contract struct and ExpirationMonth enum
- `test_csv_reader.cpp` - Tests for ContractCsvReader and PathFinder classes
- `test_data_manager.cpp` - Tests for DataManager static methods
- `test_main.cpp` - Test runner main function

### Build Configuration
- `CMakeLists.txt` - CMake configuration for building tests with GoogleTest
- `run_tests.sh` - Bash script for easy test execution

## Dependencies

The test suite uses:
- **GoogleTest (gtest)** - Unit testing framework
- **GoogleMock (gmock)** - Mocking framework
- **C++17** - Required language standard

## Running Tests

### Quick Run
```bash
cd tests
./run_tests.sh
```

### Manual Build
```bash
cd tests
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
./engine_tests
```

### With Specific Filters
```bash
# Run only TimeSeries tests
./engine_tests --gtest_filter="TimeSeriesTest.*"

# Run only CSV reader tests
./engine_tests --gtest_filter="CsvReaderTest.*"

# Run with verbose output
./engine_tests --gtest_output=xml:test_results.xml --gtest_color=yes
```

## Test Coverage

### TimeSeries Tests
- ✅ Default and parameterized constructors
- ✅ Data point access by index and timestamp
- ✅ Mutable and const accessors
- ✅ Reserve and clear functionality
- ✅ Edge cases and error handling

### Contract Tests
- ✅ Contract creation and field validation
- ✅ ExpirationMonth enum values and mappings
- ✅ Contract comparison and copying
- ✅ Various commodity types and year ranges

### CSV Reader Tests
- ✅ PathFinder path generation
- ✅ CSV reading with/without headers
- ✅ Memory-mapped vs stream reading comparison
- ✅ File error handling (not found, empty, malformed)
- ✅ Data type validation (decimals, negatives)
- ✅ Performance testing with large files

### DataManager Tests
- ✅ Contract data loading
- ✅ Non-existent contract handling
- ✅ Multiple contract loading
- ✅ Memory management verification
- ✅ Static method behavior validation

## Test Data

Tests create temporary files in `/tmp/` directories:
- `/tmp/csv_reader_test/` - CSV reader test files
- `/tmp/data_manager_test/` - DataManager test files

All test data is automatically cleaned up after test execution.

## Expected Behavior

### Known Limitations
1. **DataManager Path Configuration**: Tests expect configurable data paths, but current implementation uses hardcoded `/data/contracts/` path
2. **Error Handling**: Some tests verify that exceptions are handled gracefully rather than requiring specific error codes
3. **File System Dependencies**: Tests require write access to `/tmp/` directory

### Performance Benchmarks
- CSV reading: ~1000 rows should load in < 100ms
- Memory operations: TimeSeries operations should be sub-millisecond
- File I/O: Both mmap and stream readers should have comparable performance

## Integration with CI/CD

The test suite generates XML output compatible with most CI systems:
```bash
./engine_tests --gtest_output=xml:test_results.xml
```

Exit codes:
- `0` - All tests passed
- `1` - One or more tests failed
- Other - Build or configuration errors

## Adding New Tests

1. Create new test file: `test_new_feature.cpp`
2. Add to `CMakeLists.txt` in the `add_executable` section
3. Follow existing naming conventions:
   - Test class: `NewFeatureTest`
   - Test methods: `TEST_F(NewFeatureTest, DescriptiveTestName)`
4. Include appropriate setup/teardown in `SetUp()`/`TearDown()`

## Debugging Tests

### Debug Build
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-g -O0"
```

### Running with GDB
```bash
gdb ./engine_tests
(gdb) run --gtest_filter="YourTest.*"
```

### Valgrind Memory Check
```bash
valgrind --leak-check=full ./engine_tests
```