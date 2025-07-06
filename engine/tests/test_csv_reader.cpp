#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <fstream>
#include <filesystem>

#include "Contract.hpp"
#include "ContractCsvReader.hpp"
#include "TimeSeries.hpp"

class CsvReaderTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Create test directory
    test_dir = "/tmp/csv_reader_test";
    std::filesystem::create_directories(test_dir);
    
    // Create test CSV content (format: timestamp,close,open,high,low,volume)
    test_csv_content = 
        "timestamp,close,open,high,low,volume\n"
        "2025-01-01 09:00:00,104.0,100.0,105.0,99.0,1000\n"
        "2025-01-01 10:00:00,107.0,104.0,108.0,103.0,1100\n"
        "2025-01-01 11:00:00,109.0,107.0,110.0,106.0,1200\n"
        "2025-01-01 12:00:00,111.0,109.0,112.0,108.0,1300\n";
    
    test_csv_no_header = 
        "2025-01-01 09:00:00,104.0,100.0,105.0,99.0,1000\n"
        "2025-01-01 10:00:00,107.0,104.0,108.0,103.0,1100\n"
        "2025-01-01 11:00:00,109.0,107.0,110.0,106.0,1200\n";
    
    malformed_csv = 
        "timestamp,close,open,high,low,volume\n"
        "2025-01-01 09:00:00,104.0,100.0,105.0\n"  // Missing fields
        "invalid_timestamp,107.0,104.0,108.0,103.0,1100\n"  // Invalid timestamp
        "2025-01-01 11:00:00,109.0,107.0,110.0,106.0,1200\n";
  }
  
  void TearDown() override {
    // Clean up test files
    std::filesystem::remove_all(test_dir);
  }
  
  void CreateTestFile(const std::string& filename, const std::string& content) {
    std::string filepath = test_dir + "/" + filename;
    std::ofstream file(filepath);
    file << content;
    file.close();
  }
  
  std::string test_dir;
  std::string test_csv_content;
  std::string test_csv_no_header;
  std::string malformed_csv;
  ContractCsvReader reader;
};

// Test PathFinder functionality
TEST_F(CsvReaderTest, PathFinderGeneratesCorrectPath) {
  Contract corn_contract = {"ZC", ExpirationMonth::H, 2025};
  
  std::string expected_path = "/home/ruben/Development/SA/alchemath/data/contracts/ZC/H/2025.csv";
  std::string actual_path = PathFinder::find_contract_csv(corn_contract);
  
  EXPECT_EQ(actual_path, expected_path);
}

TEST_F(CsvReaderTest, PathFinderDifferentContracts) {
  Contract soy_may = {"ZS", ExpirationMonth::K, 2024};
  Contract wheat_dec = {"ZW", ExpirationMonth::Z, 2025};
  
  std::string soy_path = PathFinder::find_contract_csv(soy_may);
  std::string wheat_path = PathFinder::find_contract_csv(wheat_dec);
  
  EXPECT_EQ(soy_path, "/home/ruben/Development/SA/alchemath/data/contracts/ZS/K/2024.csv");
  EXPECT_EQ(wheat_path, "/home/ruben/Development/SA/alchemath/data/contracts/ZW/Z/2025.csv");
}

// Test CSV reading with header
TEST_F(CsvReaderTest, ReadCsvStreamWithHeader) {
  CreateTestFile("test_with_header.csv", test_csv_content);
  
  TimeSeries data;
  bool result = reader.read_csv_stream(test_dir + "/test_with_header.csv", data, true);
  
  EXPECT_TRUE(result);
  EXPECT_EQ(data.Timestamps().size(), 4);
  EXPECT_EQ(data.Opens().size(), 4);
  EXPECT_EQ(data.Highs().size(), 4);
  EXPECT_EQ(data.Lows().size(), 4);
  EXPECT_EQ(data.Closes().size(), 4);
  EXPECT_EQ(data.Volumes().size(), 4);
  
  // Check first data point
  EXPECT_EQ(data.Opens()[0], 100.0);
  EXPECT_EQ(data.Highs()[0], 105.0);
  EXPECT_EQ(data.Lows()[0], 99.0);
  EXPECT_EQ(data.Closes()[0], 104.0);
  EXPECT_EQ(data.Volumes()[0], 1000.0);
  
  // Check last data point
  EXPECT_EQ(data.Opens()[3], 109.0);
  EXPECT_EQ(data.Highs()[3], 112.0);
  EXPECT_EQ(data.Lows()[3], 108.0);
  EXPECT_EQ(data.Closes()[3], 111.0);
  EXPECT_EQ(data.Volumes()[3], 1300.0);
}

// Test CSV reading without header
TEST_F(CsvReaderTest, ReadCsvStreamWithoutHeader) {
  CreateTestFile("test_no_header.csv", test_csv_no_header);
  
  TimeSeries data;
  bool result = reader.read_csv_stream(test_dir + "/test_no_header.csv", data, false);
  
  EXPECT_TRUE(result);
  EXPECT_EQ(data.Timestamps().size(), 3);
  
  // Check data integrity
  EXPECT_EQ(data.Opens()[0], 100.0);
  EXPECT_EQ(data.Closes()[2], 109.0);
}

// Test memory mapped reading
TEST_F(CsvReaderTest, ReadCsvMmapWithHeader) {
  CreateTestFile("test_mmap.csv", test_csv_content);
  
  TimeSeries data;
  bool result = reader.read_csv_mmap(test_dir + "/test_mmap.csv", data, true);
  
  EXPECT_TRUE(result);
  EXPECT_EQ(data.Timestamps().size(), 4);
  
  // Compare with stream reading results
  TimeSeries stream_data;
  reader.read_csv_stream(test_dir + "/test_mmap.csv", stream_data, true);
  
  EXPECT_EQ(data.Timestamps().size(), stream_data.Timestamps().size());
  for (size_t i = 0; i < data.Timestamps().size(); ++i) {
    EXPECT_EQ(data.Opens()[i], stream_data.Opens()[i]);
    EXPECT_EQ(data.Highs()[i], stream_data.Highs()[i]);
    EXPECT_EQ(data.Lows()[i], stream_data.Lows()[i]);
    EXPECT_EQ(data.Closes()[i], stream_data.Closes()[i]);
    EXPECT_EQ(data.Volumes()[i], stream_data.Volumes()[i]);
  }
}

// Test file not found
TEST_F(CsvReaderTest, ReadNonExistentFile) {
  TimeSeries data;
  bool result = reader.read_csv_stream("/non/existent/file.csv", data);
  
  EXPECT_FALSE(result);
  EXPECT_EQ(data.Timestamps().size(), 0);
}

// Test empty file
TEST_F(CsvReaderTest, ReadEmptyFile) {
  CreateTestFile("empty.csv", "");
  
  TimeSeries data;
  bool result = reader.read_csv_stream(test_dir + "/empty.csv", data);
  
  EXPECT_TRUE(result);  // Should succeed but return empty data
  EXPECT_EQ(data.Timestamps().size(), 0);
}

// Test header-only file
TEST_F(CsvReaderTest, ReadHeaderOnlyFile) {
  CreateTestFile("header_only.csv", "timestamp,close,open,high,low,volume\n");
  
  TimeSeries data;
  bool result = reader.read_csv_stream(test_dir + "/header_only.csv", data, true);
  
  EXPECT_TRUE(result);
  EXPECT_EQ(data.Timestamps().size(), 0);
}

// Test malformed CSV handling
TEST_F(CsvReaderTest, ReadMalformedCsv) {
  CreateTestFile("malformed.csv", malformed_csv);
  
  TimeSeries data;
  bool result = reader.read_csv_stream(test_dir + "/malformed.csv", data, true);
  
  // Should handle gracefully - implementation dependent
  // At minimum, should not crash
  EXPECT_NO_THROW({
    reader.read_csv_stream(test_dir + "/malformed.csv", data, true);
  });
}

// Test large file performance (basic)
TEST_F(CsvReaderTest, ReadLargeFile) {
  // Create a larger test file
  std::string large_content = "timestamp,close,open,high,low,volume\n";
  for (int i = 0; i < 1000; ++i) {
    large_content += "2025-01-01 09:00:00,104.0,100.0,105.0,99.0,1000\n";
  }
  
  CreateTestFile("large.csv", large_content);
  
  TimeSeries data;
  auto start = std::chrono::high_resolution_clock::now();
  bool result = reader.read_csv_stream(test_dir + "/large.csv", data, true);
  auto end = std::chrono::high_resolution_clock::now();
  
  EXPECT_TRUE(result);
  EXPECT_EQ(data.Timestamps().size(), 1000);
  
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Reading 1000 rows took: " << duration.count() << " ms" << std::endl;
}

// Test different data types and edge cases
TEST_F(CsvReaderTest, ReadDataWithDecimals) {
  std::string decimal_content = 
      "timestamp,close,open,high,low,volume\n"
      "2025-01-01 09:00:00,104.321,100.123,105.456,99.789,1000\n"
      "2025-01-01 10:00:00,107.444,104.111,108.222,103.333,1100\n";
  
  CreateTestFile("decimals.csv", decimal_content);
  
  TimeSeries data;
  bool result = reader.read_csv_stream(test_dir + "/decimals.csv", data, true);
  
  EXPECT_TRUE(result);
  EXPECT_EQ(data.Timestamps().size(), 2);
  
  EXPECT_DOUBLE_EQ(data.Opens()[0], 100.123);
  EXPECT_DOUBLE_EQ(data.Highs()[0], 105.456);
  EXPECT_DOUBLE_EQ(data.Lows()[0], 99.789);
  EXPECT_DOUBLE_EQ(data.Closes()[0], 104.321);
}

TEST_F(CsvReaderTest, ReadDataWithNegativeValues) {
  std::string negative_content = 
      "timestamp,close,open,high,low,volume\n"
      "2025-01-01 09:00:00,104.0,-100.0,105.0,-99.0,1000\n";
  
  CreateTestFile("negative.csv", negative_content);
  
  TimeSeries data;
  bool result = reader.read_csv_stream(test_dir + "/negative.csv", data, true);
  
  EXPECT_TRUE(result);
  EXPECT_EQ(data.Timestamps().size(), 1);
  
  EXPECT_DOUBLE_EQ(data.Opens()[0], -100.0);
  EXPECT_DOUBLE_EQ(data.Lows()[0], -99.0);
}