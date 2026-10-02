#include "cli.hpp"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace verbose {

namespace {

constexpr const char *kUsage = "Usage: verbose --lang <LANG> <NUMBER>";
constexpr const char *kTryHelp = "For more information, try '--help'.";

void print_usage_tail() { std::cerr << kUsage << "\n\n" << kTryHelp << "\n"; }

[[noreturn]] void value_error(const std::string &message) {
  std::cerr << "error: " << message << "\n\n" << kTryHelp << "\n";
  std::exit(2);
}

[[noreturn]] void unexpected_argument(const std::string &arg) {
  std::cerr << "error: unexpected argument '" << arg << "' found\n\n";
  if (!arg.empty() && arg.front() == '-') {
    std::cerr << "  tip: to pass '" << arg << "' as a value, use '-- " << arg
              << "'\n\n";
  }
  print_usage_tail();
  std::exit(2);
}

[[noreturn]] void required_missing(bool lang_missing, bool number_missing) {
  std::cerr << "error: the following required arguments were not provided:\n";
  if (lang_missing) {
    std::cerr << "  --lang <LANG>\n";
  }
  if (number_missing) {
    std::cerr << "  <NUMBER>\n";
  }
  std::cerr << "\n";
  print_usage_tail();
  std::exit(2);
}

void print_help() {
  std::cout << "Number verbalization utility\n"
               "\n"
               "Usage: verbose --lang <LANG> <NUMBER>\n"
               "\n"
               "Arguments:\n"
               "  <NUMBER>  Number to verbalize\n"
               "\n"
               "Options:\n"
               "  -l, --lang <LANG>  Language (en, ru)\n"
               "  -h, --help         Print help\n";
}

[[noreturn]] void invalid_number(const std::string &value, const char *reason) {
  value_error("invalid value '" + value + "' for '<NUMBER>': " + reason);
}

std::uint64_t parse_number(const std::string &value) {
  std::string_view digits(value);
  if (!digits.empty() && digits.front() == '+') {
    digits.remove_prefix(1);
  }
  if (digits.empty()) {
    invalid_number(value, "cannot parse integer from empty string");
  }
  for (const char c : digits) {
    if (c < '0' || c > '9') {
      invalid_number(value, "invalid digit found in string");
    }
  }
  std::uint64_t n = 0;
  for (const char c : digits) {
    const std::uint64_t digit = static_cast<std::uint64_t>(c - '0');
    if (n > (UINT64_MAX - digit) / 10) {
      invalid_number(value, "number too large to fit in target type");
    }
    n = n * 10 + digit;
  }
  return n;
}

bool has_prefix(const std::string &s, const char *prefix) {
  return s.rfind(prefix, 0) == 0;
}

} // namespace

Cli parse_args(int argc, char **argv) {
  const std::vector<std::string> args(argv + 1, argv + argc);

  std::optional<std::uint64_t> number;
  std::optional<std::string> lang;
  bool positional_only = false;

  auto take_lang_value = [&lang](std::string value) {
    lang = std::move(value);
  };

  auto missing_value = [] {
    value_error(
        "a value is required for '--lang <LANG>' but none was supplied");
  };

  for (std::size_t i = 0; i < args.size(); ++i) {
    const std::string &arg = args[i];

    if (!positional_only && arg == "--") {
      positional_only = true;
      continue;
    }

    if (!positional_only && arg == "--help") {
      print_help();
      std::exit(0);
    }
    if (!positional_only && arg == "-h") {
      print_help();
      std::exit(0);
    }

    if (!positional_only && has_prefix(arg, "--lang")) {
      if (arg.size() == 6) {
        if (i + 1 >= args.size())
          missing_value();
        take_lang_value(args[++i]);
      } else if (arg[6] == '=') {
        take_lang_value(arg.substr(7));
      } else {
        unexpected_argument(arg);
      }
      continue;
    }

    if (!positional_only && has_prefix(arg, "-l")) {
      if (arg.size() == 2) {
        if (i + 1 >= args.size())
          missing_value();
        take_lang_value(args[++i]);
      } else {
        take_lang_value(arg.substr(2));
      }
      continue;
    }

    if (!positional_only && arg.size() > 1 && arg.front() == '-') {
      unexpected_argument(arg);
    }

    if (number) {
      unexpected_argument(arg);
    }
    number = parse_number(arg);
  }

  if (!number || !lang) {
    required_missing(!lang, !number);
  }

  return Cli{*number, *lang};
}

} // namespace verbose
