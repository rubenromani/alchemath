/**
 * @file Contract.hpp
 * @brief Defines futures contract representation and expiration month utilities.
 *
 * This file contains the Contract struct and ExpirationMonth enum used
 * throughout the AlcheMath trading engine for representing futures contracts
 * and their expiration months using standard futures market notation.
 */

#ifndef CONTRACT_HPP
#define CONTRACT_HPP

#include <iostream>
#include <string>

/**
 * @enum ExpirationMonth
 * @brief Represents futures contract expiration months using standard letter codes.
 *
 * Futures contracts use specific letter codes to represent expiration months:
 * - F: January    - G: February   - H: March      - J: April
 * - K: May        - M: June       - N: July       - Q: August
 * - U: September  - V: October    - X: November   - Z: December
 *
 * Note: The letters I, L, O, P, R, S, T, W, Y are not used in futures notation.
 */
enum ExpirationMonth {
  F,  ///< January
  G,  ///< February
  H,  ///< March
  J,  ///< April
  K,  ///< May
  M,  ///< June
  N,  ///< July
  Q,  ///< August
  U,  ///< September
  V,  ///< October
  X,  ///< November
  Z   ///< December
};

/**
 * @brief Converts an ExpirationMonth enum value to its string representation.
 *
 * @param month The expiration month enum value to convert.
 * @return std::string The single-letter string representation of the month.
 *         Returns "Unknown" for invalid enum values.
 *
 * @example
 * ```cpp
 * ExpirationMonth march = ExpirationMonth::H;
 * std::string month_str = ExpirationMonthToString(march); // Returns "H"
 * ```
 */
inline std::string ExpirationMonthToString(ExpirationMonth month) {
  switch (month) {
    case F: return "F";
    case G: return "G";
    case H: return "H";
    case J: return "J";
    case K: return "K";
    case M: return "M";
    case N: return "N";
    case Q: return "Q";
    case U: return "U";
    case V: return "V";
    case X: return "X";
    case Z: return "Z";
    default: return "Unknown";
  }
};

/**
 * @struct Contract
 * @brief Represents a futures contract with symbol, expiration month, and year.
 *
 * A Contract uniquely identifies a futures contract using:
 * - A commodity symbol (e.g., "ZC" for corn, "ZS" for soybeans)
 * - An expiration month using standard futures notation
 * - An expiration year
 *
 * @example
 * ```cpp
 * // March 2025 Corn futures contract
 * Contract corn_contract{"ZC", ExpirationMonth::H, 2025};
 * 
 * // May 2025 Soybeans futures contract
 * Contract soy_contract{"ZS", ExpirationMonth::K, 2025};
 * ```
 */
struct Contract {
  std::string symbol;              ///< Commodity symbol (e.g., "ZC", "ZS", "CL")
  ExpirationMonth expirationMonth; ///< Contract expiration month
  int expirationYear;              ///< Contract expiration year
};

#endif /* CONTRACT_HPP */