#include <iostream>
#include <string>
#include <vector>

// Simple test framework
class SimpleTest {
public:
    static int tests_run;
    static int tests_passed;
    static int tests_failed;
    
    static void EXPECT_EQ(auto expected, auto actual, const std::string& test_name) {
        tests_run++;
        if (expected == actual) {
            tests_passed++;
            std::cout << "✅ PASS: " << test_name << std::endl;
        } else {
            tests_failed++;
            std::cout << "❌ FAIL: " << test_name 
                      << " (expected: " << expected << ", got: " << actual << ")" << std::endl;
        }
    }
    
    static void EXPECT_NE(auto expected, auto actual, const std::string& test_name) {
        tests_run++;
        if (expected != actual) {
            tests_passed++;
            std::cout << "✅ PASS: " << test_name << std::endl;
        } else {
            tests_failed++;
            std::cout << "❌ FAIL: " << test_name 
                      << " (expected NOT: " << expected << ", got: " << actual << ")" << std::endl;
        }
    }
    
    static void EXPECT_TRUE(bool condition, const std::string& test_name) {
        tests_run++;
        if (condition) {
            tests_passed++;
            std::cout << "✅ PASS: " << test_name << std::endl;
        } else {
            tests_failed++;
            std::cout << "❌ FAIL: " << test_name << " (expected true, got false)" << std::endl;
        }
    }
    
    static void EXPECT_FALSE(bool condition, const std::string& test_name) {
        tests_run++;
        if (!condition) {
            tests_passed++;
            std::cout << "✅ PASS: " << test_name << std::endl;
        } else {
            tests_failed++;
            std::cout << "❌ FAIL: " << test_name << " (expected false, got true)" << std::endl;
        }
    }
    
    static void print_summary() {
        std::cout << "\n===========================================" << std::endl;
        std::cout << "           TEST SUMMARY" << std::endl;
        std::cout << "===========================================" << std::endl;
        std::cout << "Total tests: " << tests_run << std::endl;
        std::cout << "Passed: " << tests_passed << std::endl;
        std::cout << "Failed: " << tests_failed << std::endl;
        
        if (tests_failed == 0) {
            std::cout << "🎉 ALL TESTS PASSED!" << std::endl;
        } else {
            std::cout << "💥 " << tests_failed << " TEST(S) FAILED!" << std::endl;
        }
        std::cout << "===========================================" << std::endl;
    }
};

// Initialize static members
int SimpleTest::tests_run = 0;
int SimpleTest::tests_passed = 0;
int SimpleTest::tests_failed = 0;

// Forward declarations
void test_timeseries();
void test_contract();

int main() {
    std::cout << "===========================================" << std::endl;
    std::cout << "     AlcheMath Engine Simple Test Suite    " << std::endl;
    std::cout << "===========================================" << std::endl;
    
    try {
        std::cout << "\n--- Running TimeSeries Tests ---" << std::endl;
        test_timeseries();
        
        std::cout << "\n--- Running Contract Tests ---" << std::endl;
        test_contract();
        
    } catch (const std::exception& e) {
        std::cout << "❌ Exception during tests: " << e.what() << std::endl;
        SimpleTest::tests_failed++;
    }
    
    SimpleTest::print_summary();
    
    return (SimpleTest::tests_failed == 0) ? 0 : 1;
}