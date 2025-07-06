/**
 * @file DataManager.cpp
 * @brief Implementation of the DataManager class for centralized data access.
 *
 * This file implements the main interface for loading financial time series data
 * from various sources with automatic path resolution and error handling.
 */

#include "include/DataManager.hpp"

#include "include/ContractCsvReader.hpp"

TimeSeries DataManager::loadContractData(const Contract& contract) {
  std::string path = PathFinder::find_contract_csv(contract);
  TimeSeries data;
  ContractCsvReader reader;
  if (!reader.read_csv_stream(path, data, true)) {
    throw std::runtime_error("Failed to load contract data from " + path);
  }
  return data;
}