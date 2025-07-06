#include "TimeSeries.hpp"
#include <iostream>

// External SimpleTest class
extern class SimpleTest;

void test_timeseries() {
    // Test data
    std::vector<uint64_t> timestamps = {1609459200000, 1609462800000, 1609466400000};
    std::vector<double> opens = {100.0, 101.0, 102.0};
    std::vector<double> highs = {105.0, 106.0, 107.0};
    std::vector<double> lows = {99.0, 100.0, 101.0};
    std::vector<double> closes = {104.0, 105.0, 106.0};
    std::vector<double> volumes = {1000.0, 1100.0, 1200.0};
    
    // Test default constructor
    {
        TimeSeries ts;
        SimpleTest::EXPECT_EQ(0ul, ts.Timestamps().size(), "Default constructor - timestamps size");
        SimpleTest::EXPECT_EQ(0ul, ts.Opens().size(), "Default constructor - opens size");
        SimpleTest::EXPECT_EQ(0ul, ts.Highs().size(), "Default constructor - highs size");
        SimpleTest::EXPECT_EQ(0ul, ts.Lows().size(), "Default constructor - lows size");
        SimpleTest::EXPECT_EQ(0ul, ts.Closes().size(), "Default constructor - closes size");
        SimpleTest::EXPECT_EQ(0ul, ts.Volumes().size(), "Default constructor - volumes size");
    }
    
    // Test parameterized constructor
    {
        TimeSeries ts(timestamps, opens, highs, lows, closes, volumes);
        SimpleTest::EXPECT_EQ(3ul, ts.Timestamps().size(), "Parameterized constructor - size");
        SimpleTest::EXPECT_EQ(1609459200000ul, ts.Timestamps()[0], "Parameterized constructor - first timestamp");
        SimpleTest::EXPECT_EQ(100.0, ts.Opens()[0], "Parameterized constructor - first open");
        SimpleTest::EXPECT_EQ(105.0, ts.Highs()[0], "Parameterized constructor - first high");
        SimpleTest::EXPECT_EQ(99.0, ts.Lows()[0], "Parameterized constructor - first low");
        SimpleTest::EXPECT_EQ(104.0, ts.Closes()[0], "Parameterized constructor - first close");
        SimpleTest::EXPECT_EQ(1000.0, ts.Volumes()[0], "Parameterized constructor - first volume");
    }
    
    // Test DataPoint by index
    {
        TimeSeries ts(timestamps, opens, highs, lows, closes, volumes);
        OHLCV point = ts.DataPoint(1);
        SimpleTest::EXPECT_EQ(1609462800000ul, point.timestamp, "DataPoint by index - timestamp");
        SimpleTest::EXPECT_EQ(101.0, point.open, "DataPoint by index - open");
        SimpleTest::EXPECT_EQ(106.0, point.high, "DataPoint by index - high");
        SimpleTest::EXPECT_EQ(100.0, point.low, "DataPoint by index - low");
        SimpleTest::EXPECT_EQ(105.0, point.close, "DataPoint by index - close");
        SimpleTest::EXPECT_EQ(1100.0, point.volume, "DataPoint by index - volume");
    }
    
    // Test mutable accessors
    {
        TimeSeries ts;
        ts.Timestamps().push_back(1609459200000);
        ts.Opens().push_back(100.0);
        ts.Highs().push_back(105.0);
        ts.Lows().push_back(99.0);
        ts.Closes().push_back(104.0);
        ts.Volumes().push_back(1000.0);
        
        SimpleTest::EXPECT_EQ(1ul, ts.Timestamps().size(), "Mutable accessors - size after push_back");
        SimpleTest::EXPECT_EQ(1609459200000ul, ts.Timestamps()[0], "Mutable accessors - timestamp value");
        SimpleTest::EXPECT_EQ(100.0, ts.Opens()[0], "Mutable accessors - open value");
    }
    
    // Test reserve and clear
    {
        TimeSeries ts(timestamps, opens, highs, lows, closes, volumes);
        SimpleTest::EXPECT_EQ(3ul, ts.Timestamps().size(), "Before clear - size");
        
        ts.reserve(1000);
        SimpleTest::EXPECT_TRUE(ts.Timestamps().capacity() >= 1000, "Reserve - capacity check");
        
        ts.clear();
        SimpleTest::EXPECT_EQ(0ul, ts.Timestamps().size(), "After clear - timestamps size");
        SimpleTest::EXPECT_EQ(0ul, ts.Opens().size(), "After clear - opens size");
        SimpleTest::EXPECT_EQ(0ul, ts.Highs().size(), "After clear - highs size");
        SimpleTest::EXPECT_EQ(0ul, ts.Lows().size(), "After clear - lows size");
        SimpleTest::EXPECT_EQ(0ul, ts.Closes().size(), "After clear - closes size");
        SimpleTest::EXPECT_EQ(0ul, ts.Volumes().size(), "After clear - volumes size");
    }
    
    // Test const accessors
    {
        const TimeSeries ts(timestamps, opens, highs, lows, closes, volumes);
        const auto& ts_timestamps = ts.Timestamps();
        const auto& ts_opens = ts.Opens();
        
        SimpleTest::EXPECT_EQ(3ul, ts_timestamps.size(), "Const accessors - timestamps size");
        SimpleTest::EXPECT_EQ(3ul, ts_opens.size(), "Const accessors - opens size");
        SimpleTest::EXPECT_EQ(1609462800000ul, ts_timestamps[1], "Const accessors - timestamp value");
        SimpleTest::EXPECT_EQ(101.0, ts_opens[1], "Const accessors - open value");
    }
}