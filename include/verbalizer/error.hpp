#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>

namespace verbose {
class VerbalizeError : public std::runtime_error {
public:
  VerbalizeError(std::uint64_t number, std::uint64_t max)
      : std::runtime_error(make_message(number, max)), number_(number),
        max_(max) {}

  std::uint64_t number() const { return number_; }
  std::uint64_t max() const { return max_; }

private:
  static std::string make_message(std::uint64_t number, std::uint64_t max) {
    return "Number " + std::to_string(number) +
           " exceeds maximum supported value " + std::to_string(max);
  }

  std::uint64_t number_;
  std::uint64_t max_;
};

} // namespace verbose
