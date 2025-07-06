/**
 * @file DataManager.hpp
 * @brief Central data management interface for the AlcheMath trading engine.
 *
 * This file provides the main interface for loading and managing financial
 * time series data from various sources. The DataManager serves as the primary
 * entry point for accessing contract data throughout the trading engine.
 */

#ifndef DATA_MANAGER_HPP
#define DATA_MANAGER_HPP

#include "Contract.hpp"
#include "TimeSeries.hpp"

/**
 * @class DataManager
 * @brief Central manager for loading and caching financial time series data.
 *
 * The DataManager provides a unified interface for accessing contract data
 * from various sources including CSV files, databases, and real-time feeds.
 * It handles the complexity of data source selection, format conversion,
 * and caching to provide optimal performance for trading applications.
 *
 * Key features:
 * - Automatic data source detection and routing
 * - Transparent caching for frequently accessed contracts
 * - Format validation and error handling
 * - Thread-safe operations for concurrent access
 *
 * @example
 * ```cpp
 * // Load March 2025 Corn futures data
 * Contract corn_contract{"ZC", ExpirationMonth::H, 2025};
 * TimeSeries data = DataManager::loadContractData(corn_contract);
 * 
 * if (!data.Timestamps().empty()) {
 *   std::cout << "Loaded " << data.Timestamps().size() << " data points\n";
 *   
 *   // Access first data point
 *   OHLCV first_point = data.DataPoint(0);
 *   std::cout << "First close price: " << first_point.close << "\n";
 * }
 * ```
 */
class DataManager {
 public:
  /**
   * @brief Loads time series data for the specified contract.
   * 
   * @param contract The futures contract for which to load data
   * @return TimeSeries The loaded time series data, or empty TimeSeries if loading fails
   * 
   * This method automatically:
   * 1. Determines the appropriate data source for the contract
   * 2. Loads the data using the most efficient available method
   * 3. Validates the data format and integrity
   * 4. Returns a properly formatted TimeSeries object
   * 
   * The method handles various data sources transparently:
   * - CSV files following standard naming conventions
   * - Database connections for real-time data
   * - Cached data for frequently accessed contracts
   * 
   * @note This method is thread-safe and can be called concurrently
   *       from multiple threads. Internal caching optimizes repeated
   *       access to the same contract data.
   * 
   * @warning If the contract data cannot be found or contains errors,
   *          an empty TimeSeries is returned. Check the result using
   *          `data.Timestamps().empty()` before processing.
   */
  static TimeSeries loadContractData(const Contract& contract);

 private:
};

#endif /* DATA_MANAGER_HPP */