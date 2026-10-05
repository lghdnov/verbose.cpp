#include "cli.hpp"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>

#include <cxxopts/cxxopts.hpp>

namespace verbose {

Cli parse_args(int argc, char **argv) {
  cxxopts::Options options("verbose", "Number verbalization utility");
  options.add_options()("l,lang", "Language (en, ru)",
                        cxxopts::value<std::string>())(
      "number", "", cxxopts::value<std::uint64_t>())("h,help", "Print help");
  options.positional_help("<NUMBER>")
      .show_positional_help()
      .parse_positional("number");

  try {
    const cxxopts::ParseResult result = options.parse(argc, argv);
    if (result.count("help")) {
      std::cout << options.help();
      std::exit(0);
    }
    return Cli{result["number"].as<std::uint64_t>(),
               result["lang"].as<std::string>()};
  } catch (const std::exception &e) {
    std::cerr << "error: " << e.what() << "\nRun with '--help' for usage.\n";
    std::exit(2);
  }
}

} // namespace verbose
