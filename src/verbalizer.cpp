#include "verbalizer/verbalizer.hpp"
#include "verbalizer/error.hpp"

#include <array>
#include <cmath>

namespace verbose {

constexpr std::uint64_t kMax = 1'000'000'000'000'000ULL;
constexpr std::size_t kMaxDigits = 15;

struct DigitMatrix {
  const std::array<unsigned, kMaxDigits> &cells;

  unsigned operator()(std::size_t scale, std::size_t column) const {
    return cells[scale * 3 + column];
  }
};

void verbalize_number(const VerbalizerBackend &lang, std::uint64_t n,
                      std::string &out) {
  if (n == 0) {
    out.append(lang.zero());
    return;
  }
  if (n >= kMax) {
    throw VerbalizeError(n, kMax);
  }

  std::array<unsigned, kMaxDigits> digits{};
  for (std::size_t i = 0, rest = n; rest; ++i, rest /= 10)
    digits[i] = rest % 10;

  const DigitMatrix matrix{digits};
  const std::size_t top_scale = static_cast<std::size_t>(std::log10(n)) / 3 + 1;

  for (auto scale = top_scale; scale-- > 0;) {
    const std::size_t hundreds = matrix(scale, 2);
    const std::size_t tens = matrix(scale, 1);
    const std::size_t units = matrix(scale, 0);

    if (!(hundreds | tens | units))
      continue;

    const auto pair =
        lang.tens_units(lang.unit_gender_for_scale(scale), tens, units);
    out.append(lang.hundred(hundreds))
        .append(pair.tens)
        .append(pair.unit)
        .append(lang.scale_form(scale, lang.plural(tens, units)));
  }
}

std::string verbalize(const VerbalizerBackend &lang, std::uint64_t n) {
  std::string out;
  out.reserve(128);
  verbalize_number(lang, n, out);
  out.pop_back();
  return out;
}

} // namespace verbose
