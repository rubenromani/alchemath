/**
 * @file TimeSeries.hpp
 * @brief High-performance time series data structures for financial market data.
 *
 * This file implements a Structure of Arrays (SoA) approach for storing OHLCV
 * (Open, High, Low, Close, Volume) time series data, optimized for cache
 * efficiency and SIMD operations in high-frequency trading applications.
 */

#ifndef TIME_SERIES_HPP
#define TIME_SERIES_HPP

#include <cstdint>
#include <vector>

/**
 * @struct OHLCV
 * @brief Represents a single data point in a financial time series.
 *
 * Contains the standard OHLCV data for a single time period (e.g., minute, hour, day).
 * All price values are stored as doubles for precision, and timestamp is stored
 * as milliseconds since epoch for high-resolution time tracking.
 */
struct OHLCV {
  uint64_t timestamp; ///< Timestamp in milliseconds since Unix epoch
  double open;        ///< Opening price for the time period
  double high;        ///< Highest price during the time period  
  double low;         ///< Lowest price during the time period
  double close;       ///< Closing price for the time period
  double volume;      ///< Trading volume during the time period
};

/**
 * @class TimeSeries
 * @brief High-performance time series container using Structure of Arrays layout.
 *
 * Implements a Structure of Arrays (SoA) design where each price component
 * (open, high, low, close, volume) is stored in separate contiguous arrays.
 * This layout provides significant performance benefits:
 * - Improved cache locality when accessing single price components
 * - Enables SIMD vectorization for mathematical operations
 * - Reduces memory bandwidth usage for partial data access
 * 
 * @example
 * ```cpp
 * // Create time series with sample data
 * std::vector<uint64_t> timestamps = {1640995200000, 1640995260000};
 * std::vector<double> opens = {100.0, 101.0};
 * std::vector<double> highs = {102.0, 103.0};
 * std::vector<double> lows = {99.0, 100.5};
 * std::vector<double> closes = {101.0, 102.5};
 * std::vector<double> volumes = {1000.0, 1500.0};
 * 
 * TimeSeries series(timestamps, opens, highs, lows, closes, volumes);
 * 
 * // Access data by index
 * OHLCV first_point = series.DataPoint(0);
 * 
 * // Access data by timestamp
 * OHLCV point = series.DataPointByTimestamp(1640995200000);
 * ```
 */
class TimeSeries {
 public:
  /**
   * @brief Constructs a TimeSeries with provided data arrays.
   * 
   * @param timestamps Vector of timestamps in milliseconds since epoch
   * @param opens Vector of opening prices
   * @param highs Vector of high prices
   * @param lows Vector of low prices  
   * @param closes Vector of closing prices
   * @param volumes Vector of trading volumes
   * 
   * @note All input vectors must have the same size.
   */
  TimeSeries(std::vector<uint64_t> timestamps, std::vector<double> opens,
             std::vector<double> highs, std::vector<double> lows,
             std::vector<double> closes, std::vector<double> volumes);

  /**
   * @brief Default constructor creates an empty time series.
   */
  TimeSeries() = default;

  /**
   * @brief Reserves memory for the specified number of data points.
   * 
   * @param capacity Number of data points to reserve memory for
   * 
   * This method reserves memory in all internal arrays to avoid repeated
   * reallocations during data insertion, improving performance for batch loading.
   */
  void reserve(size_t capacity);

  /**
   * @brief Clears all data from the time series.
   * 
   * Removes all data points while preserving allocated memory capacity.
   */
  void clear();

  /**
   * @brief Retrieves a data point by index position.
   * 
   * @param index Zero-based index of the data point to retrieve
   * @return OHLCV The data point at the specified index
   * 
   * @note No bounds checking is performed for performance reasons.
   *       Ensure index is within valid range [0, size).
   */
  const OHLCV DataPoint(size_t index) const;

  /**
   * @brief Retrieves a data point by timestamp.
   * 
   * @param timestamp The timestamp to search for (milliseconds since epoch)
   * @return OHLCV The data point with matching timestamp, or default OHLCV if not found
   * 
   * @note Uses linear search. For frequent timestamp lookups, consider
   *       maintaining a separate timestamp index.
   */
  const OHLCV DataPointByTimestamp(uint64_t timestamp) const;

  /**
   * @brief Gets read-only access to the timestamps array.
   * @return const std::vector<uint64_t>& Reference to timestamps vector
   */
  const std::vector<uint64_t> &Timestamps() const;

  /**
   * @brief Gets read-only access to the opening prices array.
   * @return const std::vector<double>& Reference to opens vector
   */
  const std::vector<double> &Opens() const;

  /**
   * @brief Gets read-only access to the high prices array.
   * @return const std::vector<double>& Reference to highs vector
   */
  const std::vector<double> &Highs() const;

  /**
   * @brief Gets read-only access to the low prices array.
   * @return const std::vector<double>& Reference to lows vector
   */
  const std::vector<double> &Lows() const;

  /**
   * @brief Gets read-only access to the closing prices array.
   * @return const std::vector<double>& Reference to closes vector
   */
  const std::vector<double> &Closes() const;

  /**
   * @brief Gets read-only access to the volumes array.
   * @return const std::vector<double>& Reference to volumes vector
   */
  const std::vector<double> &Volumes() const;

  /**
   * @brief Gets mutable access to the timestamps array.
   * @return std::vector<uint64_t>& Reference to timestamps vector
   */
  std::vector<uint64_t> &Timestamps();

  /**
   * @brief Gets mutable access to the opening prices array.
   * @return std::vector<double>& Reference to opens vector
   */
  std::vector<double> &Opens();

  /**
   * @brief Gets mutable access to the high prices array.
   * @return std::vector<double>& Reference to highs vector
   */
  std::vector<double> &Highs();

  /**
   * @brief Gets mutable access to the low prices array.
   * @return std::vector<double>& Reference to lows vector
   */
  std::vector<double> &Lows();

  /**
   * @brief Gets mutable access to the closing prices array.
   * @return std::vector<double>& Reference to closes vector
   */
  std::vector<double> &Closes();

  /**
   * @brief Gets mutable access to the volumes array.
   * @return std::vector<double>& Reference to volumes vector
   */
  std::vector<double> &Volumes();

 private:
  std::vector<uint64_t> timestamps_; ///< Timestamps in milliseconds since epoch
  std::vector<double> opens_;        ///< Opening prices
  std::vector<double> highs_;        ///< High prices
  std::vector<double> lows_;         ///< Low prices
  std::vector<double> closes_;       ///< Closing prices
  std::vector<double> volumes_;      ///< Trading volumes
};

#endif /* TIME_SERIES_HPP */