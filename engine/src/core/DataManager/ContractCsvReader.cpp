#include "include/ContractCsvReader.hpp"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

/**
 * PathFinder implementation
 * Generates standardized paths for contract CSV files.
 */
std::string PathFinder::find_contract_csv(const Contract &contract) {
  std::string path = "/home/ruben/Development/SA/alchemath/data/contracts/" + contract.symbol + "/" +
                     ExpirationMonthToString(contract.expirationMonth) + "/" +
                     std::to_string(contract.expirationYear) + ".csv";
  return path;
}

/**
 * ContractCsvReader implementation
 * High-performance CSV parsing optimized for financial data.
 */
inline double ContractCsvReader::fast_stod(const char *start,
                                           const char *end) {
  double result = 0.0;
  double sign = 1.0;
  bool decimal = false;
  double decimal_factor = 0.1;

  if (*start == '-') {
    sign = -1.0;
    start++;
  }

  while (start < end) {
    if (*start == '.') {
      decimal = true;
    } else if (*start >= '0' && *start <= '9') {
      if (decimal) {
        result += (*start - '0') * decimal_factor;
        decimal_factor *= 0.1;
      } else {
        result = result * 10.0 + (*start - '0');
      }
    }
    start++;
  }

  return result * sign;
}

inline long long ContractCsvReader::fast_stoll(const char *start,
                                               const char *end) {
  long long result = 0;
  long long sign = 1;

  if (*start == '-') {
    sign = -1;
    start++;
  }

  while (start < end && *start >= '0' && *start <= '9') {
    result = result * 10 + (*start - '0');
    start++;
  }

  return result * sign;
}

inline std::time_t ContractCsvReader::parse_timestamp(const char *start,
                                                      const char *end) {
  // Parse "2025-06-15 18:00:00"
  int year = (start[0] - '0') * 1000 + (start[1] - '0') * 100 +
             (start[2] - '0') * 10 + (start[3] - '0');
  int month = (start[5] - '0') * 10 + (start[6] - '0');
  int day = (start[8] - '0') * 10 + (start[9] - '0');
  int hour = (start[11] - '0') * 10 + (start[12] - '0');
  int minute = (start[14] - '0') * 10 + (start[15] - '0');
  int second = (start[17] - '0') * 10 + (start[18] - '0');

  std::tm tm = {};
  tm.tm_year = year - 1900;
  tm.tm_mon = month - 1;
  tm.tm_mday = day;
  tm.tm_hour = hour;
  tm.tm_min = minute;
  tm.tm_sec = second;
  tm.tm_isdst = -1;

  return std::mktime(&tm);
}

inline const char *ContractCsvReader::find_delimiter(const char *start,
                                                     const char *end,
                                                     char delimiter) {
  while (start < end && *start != delimiter) {
    start++;
  }
  return start;
}

inline const char *ContractCsvReader::skip_header(const char *data,
                                                  const char *end) {
  return find_delimiter(data, end, '\n') + 1;
}

bool ContractCsvReader::read_csv_mmap(const std::string &filename,
                                      TimeSeries &data, bool has_header) {
  int fd = open(filename.c_str(), O_RDONLY);
  if (fd == -1) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return false;
  }

  struct stat sb;
  if (fstat(fd, &sb) == -1) {
    close(fd);
    std::cerr << "Error reading file information" << std::endl;
    return false;
  }

  const char *file_data = static_cast<const char *>(
      mmap(nullptr, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0));

  if (file_data == MAP_FAILED) {
    close(fd);
    std::cerr << "Error in memory mapping" << std::endl;
    return false;
  }

  const char *current = file_data;
  const char *end = file_data + sb.st_size;

  // Skip header if present
  if (has_header) {
    current = skip_header(current, end);
  }

  // Rough estimate of number of rows for pre-allocation
  size_t estimated_rows = sb.st_size / 60;  // Estimate about 60 chars per row
  data.reserve(estimated_rows);
  data.clear();

  while (current < end) {
    // Parse timestamp
    const char *field_end = find_delimiter(current, end, ',');
    if (field_end >= end) break;

    std::time_t timestamp = parse_timestamp(current, field_end);
    data.Timestamps().push_back(timestamp);
    current = field_end + 1;

    // Parse close (second column in our data)
    field_end = find_delimiter(current, end, ',');
    if (field_end >= end) break;
    double close_val = fast_stod(current, field_end);
    data.Closes().push_back(close_val);
    current = field_end + 1;

    // Parse open (third column in our data)
    field_end = find_delimiter(current, end, ',');
    if (field_end >= end) break;
    double open_val = fast_stod(current, field_end);
    data.Opens().push_back(open_val);
    current = field_end + 1;

    // Parse high (fourth column in our data)
    field_end = find_delimiter(current, end, ',');
    if (field_end >= end) break;
    double high_val = fast_stod(current, field_end);
    data.Highs().push_back(high_val);
    current = field_end + 1;

    // Parse low (fifth column in our data)
    field_end = find_delimiter(current, end, ',');
    if (field_end >= end) break;
    double low_val = fast_stod(current, field_end);
    data.Lows().push_back(low_val);
    current = field_end + 1;

    // Parse volume (last field, ends with \n or \r\n)
    field_end = find_delimiter(current, end, '\n');
    if (field_end > current && *(field_end - 1) == '\r') {
      field_end--;  // Handle \r\n
    }
    long long volume_val = fast_stoll(current, field_end);
    data.Volumes().push_back(volume_val);

    // Go to next row
    current = find_delimiter(current, end, '\n') + 1;
  }

  munmap(const_cast<char *>(file_data), sb.st_size);
  close(fd);

  return true;
}

bool ContractCsvReader::read_csv_stream(const std::string &filename,
                                        TimeSeries &data, bool has_header) {
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return false;
  }

  // Get file size for pre-allocation
  file.seekg(0, std::ios::end);
  size_t file_size = file.tellg();
  file.seekg(0, std::ios::beg);

  // Pre-allocate vectors
  size_t estimated_rows = file_size / 60;
  data.reserve(estimated_rows);
  data.clear();

  std::string line;
  line.reserve(128);  // Pre-allocate string to avoid reallocations

  // Skip header if present
  if (has_header && std::getline(file, line)) {
    // Header skipped
  }

  while (std::getline(file, line)) {
    if (line.empty()) continue;

    const char *current = line.c_str();
    const char *end = current + line.length();

    // Parse timestamp
    const char *field_end = find_delimiter(current, end, ',');
    if (field_end >= end) continue;

    std::time_t timestamp = parse_timestamp(current, field_end);
    data.Timestamps().push_back(timestamp);
    current = field_end + 1;

    // Parse close (second column in our data)
    field_end = find_delimiter(current, end, ',');
    if (field_end >= end) continue;
    data.Closes().push_back(fast_stod(current, field_end));
    current = field_end + 1;

    // Parse open (third column in our data)
    field_end = find_delimiter(current, end, ',');
    if (field_end >= end) continue;
    data.Opens().push_back(fast_stod(current, field_end));
    current = field_end + 1;

    // Parse high (fourth column in our data)
    field_end = find_delimiter(current, end, ',');
    if (field_end >= end) continue;
    data.Highs().push_back(fast_stod(current, field_end));
    current = field_end + 1;

    // Parse low (fifth column in our data)
    field_end = find_delimiter(current, end, ',');
    if (field_end >= end) continue;
    data.Lows().push_back(fast_stod(current, field_end));
    current = field_end + 1;

    // Parse volume
    data.Volumes().push_back(fast_stoll(current, end));
  }

  return true;
}