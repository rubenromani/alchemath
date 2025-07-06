/**
 * @file TimeSeries.cpp
 * @brief Implementation of the TimeSeries class for high-performance time series data.
 *
 * This file implements the Structure of Arrays (SoA) approach for storing OHLCV
 * data with optimized memory layout and access patterns for financial applications.
 */

#include "include/TimeSeries.hpp"

#include <algorithm>
#include <stdexcept>

TimeSeries::TimeSeries(std::vector<uint64_t> timestamps,
                       std::vector<double> opens, std::vector<double> highs,
                       std::vector<double> lows, std::vector<double> closes,
                       std::vector<double> volumes)
    : timestamps_(std::move(timestamps)),
      opens_(std::move(opens)),
      highs_(std::move(highs)),
      lows_(std::move(lows)),
      closes_(std::move(closes)),
      volumes_(std::move(volumes)) {}

void TimeSeries::reserve(size_t capacity) {
  timestamps_.reserve(capacity);
  opens_.reserve(capacity);
  highs_.reserve(capacity);
  lows_.reserve(capacity);
  closes_.reserve(capacity);
  volumes_.reserve(capacity);
}

void TimeSeries::clear() {
  timestamps_.clear();
  opens_.clear();
  highs_.clear();
  lows_.clear();
  closes_.clear();
  volumes_.clear();
}

const OHLCV TimeSeries::DataPoint(size_t index) const {
  if (index >= timestamps_.size()) {
    throw std::out_of_range("Index out of range");
  }
  return OHLCV{timestamps_[index], opens_[index],  highs_[index],
               lows_[index],       closes_[index], volumes_[index]};
}

const OHLCV TimeSeries::DataPointByTimestamp(uint64_t timestamp) const {
  auto it = std::find(timestamps_.begin(), timestamps_.end(), timestamp);
  if (it == timestamps_.end()) {
    throw std::out_of_range("Timestamp not found");
  }
  size_t index = std::distance(timestamps_.begin(), it);
  return OHLCV{timestamps_[index], opens_[index],  highs_[index],
               lows_[index],       closes_[index], volumes_[index]};
}

const std::vector<uint64_t> &TimeSeries::Timestamps() const {
  return timestamps_;
}

const std::vector<double> &TimeSeries::Opens() const { return opens_; }

const std::vector<double> &TimeSeries::Highs() const { return highs_; }

const std::vector<double> &TimeSeries::Lows() const { return lows_; }

const std::vector<double> &TimeSeries::Closes() const { return closes_; }

const std::vector<double> &TimeSeries::Volumes() const { return volumes_; }

std::vector<uint64_t> &TimeSeries::Timestamps() { return timestamps_; }

std::vector<double> &TimeSeries::Opens() { return opens_; }

std::vector<double> &TimeSeries::Highs() { return highs_; }

std::vector<double> &TimeSeries::Lows() { return lows_; }

std::vector<double> &TimeSeries::Closes() { return closes_; }

std::vector<double> &TimeSeries::Volumes() { return volumes_; }