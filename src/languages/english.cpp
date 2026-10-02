#include <array>
#include <cstddef>

#include "verbalizer/verbalizer.hpp"

namespace verbose {
namespace {

constexpr std::array<const char *, 10> kUnits = {
    "",      "one ", "two ",   "three ", "four ",
    "five ", "six ", "seven ", "eight ", "nine ",
};

constexpr std::array<const char *, 10> kTeens = {
    "ten ",     "eleven ",  "twelve ",    "thirteen ", "fourteen ",
    "fifteen ", "sixteen ", "seventeen ", "eighteen ", "nineteen ",
};

constexpr std::array<const char *, 10> kTens = {
    "",       "",       "twenty ",  "thirty ", "forty ",
    "fifty ", "sixty ", "seventy ", "eighty ", "ninety ",
};

constexpr std::array<const char *, 10> kHundreds = {
    "",
    "one hundred ",
    "two hundred ",
    "three hundred ",
    "four hundred ",
    "five hundred ",
    "six hundred ",
    "seven hundred ",
    "eight hundred ",
    "nine hundred ",
};

constexpr std::array<const std::array<const char *, 5>, 3> kScaleWords = {{
    {{"", "thousand ", "million ", "billion ", "trillion "}},
    {{"", "thousand ", "million ", "billion ", "trillion "}},
    {{"", "thousand ", "million ", "billion ", "trillion "}},
}};

const TensUnitsTable kTensUnits = make_tens_units_table(kUnits, kTeens, kTens);

const PluralTable kPlural = [] {
  PluralTable table{};
  for (std::size_t tens = 0; tens < 10; ++tens) {
    for (std::size_t units = 0; units < 10; ++units) {
      table[tens][units] =
          (tens == 0 && units == 1) ? PluralForm::One : PluralForm::Many;
    }
  }
  return table;
}();

class EnglishVerbalizer : public VerbalizerBackend {
public:
  const char *code() const override { return "en"; }
  const char *name() const override { return "English"; }
  const char *zero() const override { return "zero"; }

  const char *hundred(std::size_t digit) const override {
    return kHundreds[digit];
  }

  TensUnitsWord tens_units(Gender, std::size_t tens_digit,
                           std::size_t units_digit) const override {
    return kTensUnits[tens_digit][units_digit];
  }

  const char *scale_form(std::size_t scale_idx,
                         PluralForm form) const override {
    return scale_idx < kScaleWords[0].size()
               ? kScaleWords[static_cast<std::size_t>(form)][scale_idx]
               : "";
  }

  PluralForm plural(std::size_t tens_digit,
                    std::size_t units_digit) const override {
    return kPlural[tens_digit][units_digit];
  }

  Gender unit_gender_for_scale(std::size_t) const override {
    return Gender::Masc;
  }
};

const EnglishVerbalizer kEnglishVerbalizer;
const RegisterVerbalizer kEnglishRegistration{kEnglishVerbalizer};

} // namespace
} // namespace verbose
