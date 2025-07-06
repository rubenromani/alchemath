/**
 * @file ContractCsvReader.hpp
 * @brief High-performance CSV reader for futures contract OHLCV data.
 *
 * This file provides optimized CSV reading capabilities for financial time series data,
 * supporting both memory-mapped file I/O and stream-based reading. The implementation
 * is designed for maximum performance when processing large datasets of market data.
 */

#ifndef CSV_READER_HPP
#define CSV_READER_HPP

#include <ctime>
#include <string>

#include "Contract.hpp"
#include "TimeSeries.hpp"

/**
 * @class PathFinder
 * @brief Utility class for generating standardized file paths for contract data.
 *
 * Provides methods to generate file system paths for contract CSV files
 * following the standard directory structure used by the AlcheMath engine.
 */
class PathFinder {
 public:
  /**
   * @brief Generates the file path for a contract's CSV data file.
   * 
   * @param contract The contract for which to generate the file path
   * @return std::string The complete file path to the contract's CSV data
   * 
   * The path format follows the pattern:
   * `/data/contracts/{symbol}/{month_letter}/{year}.csv`
   * 
   * @example
   * ```cpp
   * Contract corn_contract{"ZC", ExpirationMonth::H, 2025};
   * std::string path = PathFinder::find_contract_csv(corn_contract);
   * // Result: "/data/contracts/ZC/H/2025.csv"
   * ```
   */
  static std::string find_contract_csv(const Contract &contract);
};

/**
 * @class ContractCsvReader
 * @brief High-performance CSV reader optimized for financial time series data.
 *
 * Provides two reading methods:
 * 1. Memory-mapped I/O (read_csv_mmap) - Fastest for large files, maps file into memory
 * 2. Stream-based I/O (read_csv_stream) - More memory-efficient for smaller files
 *
 * Both methods use custom parsing algorithms optimized for the specific format
 * of OHLCV data, providing significant performance improvements over standard
 * library parsing functions.
 *
 * Expected CSV format:
 * - Columns: timestamp, close, open, high, low, volume
 * - Timestamp format: ISO date string (YYYY-MM-DD HH:MM:SS)
 * - Numeric precision: Double precision floating point
 * - Optional header row
 *
 * @example
 * ```cpp
 * ContractCsvReader reader;
 * TimeSeries data;
 * 
 * // Read using memory-mapped I/O (recommended for large files)
 * if (reader.read_csv_mmap("path/to/data.csv", data, true)) {
 *   // Process the loaded time series data
 *   std::cout << "Loaded " << data.Timestamps().size() << " data points\n";
 * }
 * ```
 */
class ContractCsvReader {
 private:
  /**
   * @brief Fast string-to-double conversion optimized for CSV parsing.
   * 
   * @param start Pointer to the start of the number string
   * @param end Pointer to one past the end of the number string
   * @return double The parsed double value
   * 
   * Custom implementation that avoids locale-dependent parsing and
   * provides better performance than std::stod for CSV data.
   */
  inline double fast_stod(const char *start, const char *end);

  /**
   * @brief Fast string-to-long-long conversion for timestamp parsing.
   * 
   * @param start Pointer to the start of the number string
   * @param end Pointer to one past the end of the number string
   * @return long long The parsed integer value
   * 
   * Optimized for parsing Unix timestamps and other integer values
   * commonly found in financial data files.
   */
  inline long long fast_stoll(const char *start, const char *end);

  /**
   * @brief Parses timestamp strings into std::time_t values.
   * 
   * @param start Pointer to the start of the timestamp string
   * @param end Pointer to one past the end of the timestamp string
   * @return std::time_t The parsed timestamp
   * 
   * Supports multiple timestamp formats commonly used in financial data:
   * - Unix timestamps (seconds since epoch)
   * - ISO 8601 date strings
   * - Custom date formats
   */
  inline std::time_t parse_timestamp(const char *start, const char *end);

  /**
   * @brief Finds the next occurrence of a delimiter character.
   * 
   * @param start Pointer to start searching from
   * @param end Pointer to the end of the search range
   * @param delimiter The character to search for (typically ',' or '\t')
   * @return const char* Pointer to the delimiter, or end if not found
   * 
   * Optimized for CSV parsing with minimal overhead.
   */
  inline const char *find_delimiter(const char *start, const char *end,
                                    char delimiter);

  /**
   * @brief Skips the header row in CSV data.
   * 
   * @param data Pointer to the start of the CSV data
   * @param end Pointer to the end of the CSV data
   * @return const char* Pointer to the start of the first data row
   * 
   * Efficiently locates the first newline character to skip header information.
   */
  inline const char *skip_header(const char *data, const char *end);

 public:
  /**
   * @brief Reads CSV data using memory-mapped file I/O.
   * 
   * @param filename Path to the CSV file to read
   * @param data TimeSeries object to populate with parsed data
   * @param has_header Whether the CSV file contains a header row (default: true)
   * @return bool True if reading was successful, false on error
   * 
   * Memory-mapped I/O provides the best performance for large files by mapping
   * the entire file into virtual memory. This avoids copying data and allows
   * the OS to optimize memory access patterns.
   * 
   * @note This method may use significant virtual memory for very large files.
   */
  bool read_csv_mmap(const std::string &filename, TimeSeries &data,
                     bool has_header = true);

  /**
   * @brief Reads CSV data using traditional stream-based I/O.
   * 
   * @param filename Path to the CSV file to read
   * @param data TimeSeries object to populate with parsed data
   * @param has_header Whether the CSV file contains a header row (default: true)
   * @return bool True if reading was successful, false on error
   * 
   * Stream-based reading is more memory-efficient and suitable for smaller files
   * or systems with limited memory. Uses buffered I/O for good performance.
   * 
   * @note Generally slower than memory-mapped I/O for large files.
   */
  bool read_csv_stream(const std::string &filename, TimeSeries &data,
                       bool has_header = true);
};

#endif /* CSV_READER_HPP */