#include <gtest/gtest.h>

#include "Contract.hpp"

class ContractTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Setup test contracts
    corn_contract = {"ZC", ExpirationMonth::H, 2025};  // March 2025 Corn
    soy_contract = {"ZS", ExpirationMonth::K, 2025};   // May 2025 Soybeans
    wheat_contract = {"ZW", ExpirationMonth::Z, 2024}; // December 2024 Wheat
  }

  Contract corn_contract;
  Contract soy_contract;
  Contract wheat_contract;
};

TEST_F(ContractTest, ContractCreation) {
  EXPECT_EQ(corn_contract.symbol, "ZC");
  EXPECT_EQ(corn_contract.expirationMonth, ExpirationMonth::H);
  EXPECT_EQ(corn_contract.expirationYear, 2025);
}

TEST_F(ContractTest, ExpirationMonthValues) {
  // Test all expiration month enum values
  EXPECT_EQ(static_cast<int>(ExpirationMonth::F), 0);  // January
  EXPECT_EQ(static_cast<int>(ExpirationMonth::G), 1);  // February
  EXPECT_EQ(static_cast<int>(ExpirationMonth::H), 2);  // March
  EXPECT_EQ(static_cast<int>(ExpirationMonth::J), 3);  // April
  EXPECT_EQ(static_cast<int>(ExpirationMonth::K), 4);  // May
  EXPECT_EQ(static_cast<int>(ExpirationMonth::M), 5);  // June
  EXPECT_EQ(static_cast<int>(ExpirationMonth::N), 6);  // July
  EXPECT_EQ(static_cast<int>(ExpirationMonth::Q), 7);  // August
  EXPECT_EQ(static_cast<int>(ExpirationMonth::U), 8);  // September
  EXPECT_EQ(static_cast<int>(ExpirationMonth::V), 9);  // October
  EXPECT_EQ(static_cast<int>(ExpirationMonth::X), 10); // November
  EXPECT_EQ(static_cast<int>(ExpirationMonth::Z), 11); // December
}

TEST_F(ContractTest, ContractComparison) {
  Contract another_corn = {"ZC", ExpirationMonth::H, 2025};
  Contract different_year = {"ZC", ExpirationMonth::H, 2024};
  Contract different_month = {"ZC", ExpirationMonth::K, 2025};
  Contract different_symbol = {"ZS", ExpirationMonth::H, 2025};
  
  // Test equality (manual comparison since no operator== defined)
  EXPECT_EQ(corn_contract.symbol, another_corn.symbol);
  EXPECT_EQ(corn_contract.expirationMonth, another_corn.expirationMonth);
  EXPECT_EQ(corn_contract.expirationYear, another_corn.expirationYear);
  
  // Test differences
  EXPECT_NE(corn_contract.expirationYear, different_year.expirationYear);
  EXPECT_NE(corn_contract.expirationMonth, different_month.expirationMonth);
  EXPECT_NE(corn_contract.symbol, different_symbol.symbol);
}

TEST_F(ContractTest, ContractCopyAssignment) {
  Contract copy_contract = corn_contract;
  
  EXPECT_EQ(copy_contract.symbol, corn_contract.symbol);
  EXPECT_EQ(copy_contract.expirationMonth, corn_contract.expirationMonth);
  EXPECT_EQ(copy_contract.expirationYear, corn_contract.expirationYear);
  
  // Modify copy and ensure original is unchanged
  copy_contract.symbol = "ZW";
  copy_contract.expirationMonth = ExpirationMonth::Z;
  copy_contract.expirationYear = 2024;
  
  EXPECT_EQ(corn_contract.symbol, "ZC");
  EXPECT_EQ(corn_contract.expirationMonth, ExpirationMonth::H);
  EXPECT_EQ(corn_contract.expirationYear, 2025);
}

TEST_F(ContractTest, VariousContractTypes) {
  // Test different commodity symbols
  Contract gold = {"GC", ExpirationMonth::G, 2025};
  Contract crude_oil = {"CL", ExpirationMonth::M, 2025};
  Contract natural_gas = {"NG", ExpirationMonth::U, 2025};
  
  EXPECT_EQ(gold.symbol, "GC");
  EXPECT_EQ(crude_oil.symbol, "CL");
  EXPECT_EQ(natural_gas.symbol, "NG");
  
  EXPECT_EQ(gold.expirationMonth, ExpirationMonth::G);
  EXPECT_EQ(crude_oil.expirationMonth, ExpirationMonth::M);
  EXPECT_EQ(natural_gas.expirationMonth, ExpirationMonth::U);
}

TEST_F(ContractTest, ExpirationYearRange) {
  // Test various year ranges
  Contract historical = {"ZC", ExpirationMonth::H, 2020};
  Contract current = {"ZC", ExpirationMonth::H, 2024};
  Contract future = {"ZC", ExpirationMonth::H, 2030};
  
  EXPECT_EQ(historical.expirationYear, 2020);
  EXPECT_EQ(current.expirationYear, 2024);
  EXPECT_EQ(future.expirationYear, 2030);
  
  EXPECT_LT(historical.expirationYear, current.expirationYear);
  EXPECT_LT(current.expirationYear, future.expirationYear);
}

TEST_F(ContractTest, ExpirationMonthToString) {
  // Test ExpirationMonthToString function for all months
  EXPECT_EQ(ExpirationMonthToString(ExpirationMonth::F), "F");   // January
  EXPECT_EQ(ExpirationMonthToString(ExpirationMonth::G), "G");   // February
  EXPECT_EQ(ExpirationMonthToString(ExpirationMonth::H), "H");   // March
  EXPECT_EQ(ExpirationMonthToString(ExpirationMonth::J), "J");   // April
  EXPECT_EQ(ExpirationMonthToString(ExpirationMonth::K), "K");   // May
  EXPECT_EQ(ExpirationMonthToString(ExpirationMonth::M), "M");   // June
  EXPECT_EQ(ExpirationMonthToString(ExpirationMonth::N), "N");   // July
  EXPECT_EQ(ExpirationMonthToString(ExpirationMonth::Q), "Q");   // August
  EXPECT_EQ(ExpirationMonthToString(ExpirationMonth::U), "U");   // September
  EXPECT_EQ(ExpirationMonthToString(ExpirationMonth::V), "V");   // October
  EXPECT_EQ(ExpirationMonthToString(ExpirationMonth::X), "X");   // November
  EXPECT_EQ(ExpirationMonthToString(ExpirationMonth::Z), "Z");   // December
}

TEST_F(ContractTest, ExpirationMonthToStringWithContracts) {
  // Test the function in context with actual contracts
  EXPECT_EQ(ExpirationMonthToString(corn_contract.expirationMonth), "H");   // March
  EXPECT_EQ(ExpirationMonthToString(soy_contract.expirationMonth), "K");    // May
  EXPECT_EQ(ExpirationMonthToString(wheat_contract.expirationMonth), "Z");  // December
}