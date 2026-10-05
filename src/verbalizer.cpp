#include "verbalizer/verbalizer.hpp"
#include "verbalizer/error.hpp"

#include <array>

namespace verbose {

constexpr std::uint64_t kMax = 1'000'000'000'000'000ULL;
constexpr std::size_t kMaxDigits = 15;

void verbalize_number(const VerbalizerBackend &lang, std::uint64_t n,
                      std::string &out) {
  if (n == 0) {
    out.append(lang.zero());
    return;
  }
  if (n >= kMax) {
    throw VerbalizeError(n, kMax);
  }

  std::array<uint8_t, kMaxDigits> digits{};
  std::size_t count = 0;
  for (std::size_t rest = n; rest; rest /= 10)
    digits[count++] = rest % 10;

  const std::size_t top_scale = (count - 1) / 3 + 1;

  for (auto scale = top_scale; scale-- > 0;) {
    const std::size_t base = scale * 3;
    const std::size_t units = digits[base];
    const std::size_t tens = digits[base + 1];
    const std::size_t hundreds = digits[base + 2];

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
