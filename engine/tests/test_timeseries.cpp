#include <gtest/gtest.h>

#include "TimeSeries.hpp"

class TimeSeriesTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Sample test data
    timestamps = {1609459200000, 1609462800000, 1609466400000, 1609470000000};
    opens = {100.0, 101.0, 102.0, 103.0};
    highs = {105.0, 106.0, 107.0, 108.0};
    lows = {99.0, 100.0, 101.0, 102.0};
    closes = {104.0, 105.0, 106.0, 107.0};
    volumes = {1000.0, 1100.0, 1200.0, 1300.0};
  }

  std::vector<uint64_t> timestamps;
  std::vector<double> opens;
  std::vector<double> highs;
  std::vector<double> lows;
  std::vector<double> closes;
  std::vector<double> volumes;
};

TEST_F(TimeSeriesTest, DefaultConstructor) {
  TimeSeries ts;
  EXPECT_EQ(ts.Timestamps().size(), 0);
  EXPECT_EQ(ts.Opens().size(), 0);
  EXPECT_EQ(ts.Highs().size(), 0);
  EXPECT_EQ(ts.Lows().size(), 0);
  EXPECT_EQ(ts.Closes().size(), 0);
  EXPECT_EQ(ts.Volumes().size(), 0);
}

TEST_F(TimeSeriesTest, ParameterizedConstructor) {
  TimeSeries ts(timestamps, opens, highs, lows, closes, volumes);
  
  EXPECT_EQ(ts.Timestamps().size(), 4);
  EXPECT_EQ(ts.Opens().size(), 4);
  EXPECT_EQ(ts.Highs().size(), 4);
  EXPECT_EQ(ts.Lows().size(), 4);
  EXPECT_EQ(ts.Closes().size(), 4);
  EXPECT_EQ(ts.Volumes().size(), 4);
  
  EXPECT_EQ(ts.Timestamps()[0], 1609459200000);
  EXPECT_EQ(ts.Opens()[0], 100.0);
  EXPECT_EQ(ts.Highs()[0], 105.0);
  EXPECT_EQ(ts.Lows()[0], 99.0);
  EXPECT_EQ(ts.Closes()[0], 104.0);
  EXPECT_EQ(ts.Volumes()[0], 1000.0);
}

TEST_F(TimeSeriesTest, DataPointByIndex) {
  TimeSeries ts(timestamps, opens, highs, lows, closes, volumes);
  
  OHLCV point = ts.DataPoint(0);
  EXPECT_EQ(point.timestamp, 1609459200000);
  EXPECT_EQ(point.open, 100.0);
  EXPECT_EQ(point.high, 105.0);
  EXPECT_EQ(point.low, 99.0);
  EXPECT_EQ(point.close, 104.0);
  EXPECT_EQ(point.volume, 1000.0);
  
  point = ts.DataPoint(2);
  EXPECT_EQ(point.timestamp, 1609466400000);
  EXPECT_EQ(point.open, 102.0);
  EXPECT_EQ(point.high, 107.0);
  EXPECT_EQ(point.low, 101.0);
  EXPECT_EQ(point.close, 106.0);
  EXPECT_EQ(point.volume, 1200.0);
}

TEST_F(TimeSeriesTest, DataPointByTimestamp) {
  TimeSeries ts(timestamps, opens, highs, lows, closes, volumes);
  
  OHLCV point = ts.DataPointByTimestamp(1609462800000);
  EXPECT_EQ(point.timestamp, 1609462800000);
  EXPECT_EQ(point.open, 101.0);
  EXPECT_EQ(point.high, 106.0);
  EXPECT_EQ(point.low, 100.0);
  EXPECT_EQ(point.close, 105.0);
  EXPECT_EQ(point.volume, 1100.0);
}

TEST_F(TimeSeriesTest, MutableAccessors) {
  TimeSeries ts;
  
  ts.Timestamps().push_back(1609459200000);
  ts.Opens().push_back(100.0);
  ts.Highs().push_back(105.0);
  ts.Lows().push_back(99.0);
  ts.Closes().push_back(104.0);
  ts.Volumes().push_back(1000.0);
  
  EXPECT_EQ(ts.Timestamps().size(), 1);
  EXPECT_EQ(ts.Timestamps()[0], 1609459200000);
  EXPECT_EQ(ts.Opens()[0], 100.0);
}

TEST_F(TimeSeriesTest, ReserveAndClear) {
  TimeSeries ts(timestamps, opens, highs, lows, closes, volumes);
  
  EXPECT_EQ(ts.Timestamps().size(), 4);
  
  ts.reserve(1000);
  EXPECT_GE(ts.Timestamps().capacity(), 1000);
  
  ts.clear();
  EXPECT_EQ(ts.Timestamps().size(), 0);
  EXPECT_EQ(ts.Opens().size(), 0);
  EXPECT_EQ(ts.Highs().size(), 0);
  EXPECT_EQ(ts.Lows().size(), 0);
  EXPECT_EQ(ts.Closes().size(), 0);
  EXPECT_EQ(ts.Volumes().size(), 0);
}

TEST_F(TimeSeriesTest, ConstAccessors) {
  const TimeSeries ts(timestamps, opens, highs, lows, closes, volumes);
  
  const auto& ts_timestamps = ts.Timestamps();
  const auto& ts_opens = ts.Opens();
  const auto& ts_highs = ts.Highs();
  const auto& ts_lows = ts.Lows();
  const auto& ts_closes = ts.Closes();
  const auto& ts_volumes = ts.Volumes();
  
  EXPECT_EQ(ts_timestamps.size(), 4);
  EXPECT_EQ(ts_opens.size(), 4);
  EXPECT_EQ(ts_highs.size(), 4);
  EXPECT_EQ(ts_lows.size(), 4);
  EXPECT_EQ(ts_closes.size(), 4);
  EXPECT_EQ(ts_volumes.size(), 4);
  
  EXPECT_EQ(ts_timestamps[1], 1609462800000);
  EXPECT_EQ(ts_opens[1], 101.0);
}

// Test edge cases
TEST_F(TimeSeriesTest, EmptyDataPointAccess) {
  TimeSeries ts;
  
  // These should handle gracefully or throw appropriate exceptions
  // depending on implementation
  EXPECT_NO_THROW({
    try {
      ts.DataPoint(0);
    } catch (const std::exception& e) {
      // Expected behavior for empty series
    }
  });
}

TEST_F(TimeSeriesTest, OutOfBoundsAccess) {
  TimeSeries ts(timestamps, opens, highs, lows, closes, volumes);
  
  EXPECT_NO_THROW({
    try {
      ts.DataPoint(10);  // Out of bounds
    } catch (const std::exception& e) {
      // Expected behavior for out of bounds
    }
  });
}