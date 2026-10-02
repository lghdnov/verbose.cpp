#include <iostream>

#include "cli.hpp"
#include "verbalizer/error.hpp"
#include "verbalizer/verbalizer.hpp"

int main(int argc, char **argv) {
  const verbose::Cli args = verbose::parse_args(argc, argv);
  const verbose::VerbalizerRegistry &registry =
      verbose::VerbalizerRegistry::instance();

  const verbose::VerbalizerBackend *verbalizer = registry.get(args.lang);
  if (verbalizer == nullptr) {
    std::cerr << "Error: unknown language '" << args.lang
              << "'. Available: " << registry.codes_string() << "\n";
    return 1;
  }

  try {
    std::cout << verbose::verbalize(*verbalizer, args.number) << "\n";
  } catch (const verbose::VerbalizeError &e) {
    std::cerr << "Error: " << e.what() << ".\n";
    return 1;
  }
  return 0;
}
