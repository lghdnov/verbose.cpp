#pragma once

#include <cstdint>
#include <string>

namespace verbose {

struct Cli {
  std::uint64_t number;
  std::string lang;
};

Cli parse_args(int argc, char **argv);

} // namespace verbose
