#include <array>
#include <cstddef>

#include "verbalizer/verbalizer.hpp"

namespace verbose {
namespace {

constexpr std::array<const char *, 10> kUnitsMasc = {
    "",      "один ",  "два ",  "три ",    "четыре ",
    "пять ", "шесть ", "семь ", "восемь ", "девять ",
};

constexpr std::array<const char *, 10> kUnitsFem = {
    "",      "одна ",  "две ",  "три ",    "четыре ",
    "пять ", "шесть ", "семь ", "восемь ", "девять ",
};

constexpr std::array<const char *, 10> kTeens = {
    "десять ",       "одиннадцать ",  "двенадцать ",  "тринадцать ",
    "четырнадцать ", "пятнадцать ",   "шестнадцать ", "семнадцать ",
    "восемнадцать ", "девятнадцать ",
};

constexpr std::array<const char *, 10> kTens = {
    "",
    "",
    "двадцать ",
    "тридцать ",
    "сорок ",
    "пятьдесят ",
    "шестьдесят ",
    "семьдесят ",
    "восемьдесят ",
    "девяносто ",
};

constexpr std::array<const char *, 10> kHundreds = {
    "",         "сто ",      "двести ",  "триста ",    "четыреста ",
    "пятьсот ", "шестьсот ", "семьсот ", "восемьсот ", "девятьсот ",
};

constexpr std::array<const std::array<const char *, 5>, 3> kScaleWords = {{
    {{"", "тысяча ", "миллион ", "миллиард ", "триллион "}},
    {{"", "тысячи ", "миллиона ", "миллиарда ", "триллиона "}},
    {{"", "тысяч ", "миллионов ", "миллиардов ", "триллионов "}},
}};

const TensUnitsTable kTensUnitsMasc =
    make_tens_units_table(kUnitsMasc, kTeens, kTens);
const TensUnitsTable kTensUnitsFem =
    make_tens_units_table(kUnitsFem, kTeens, kTens);

const PluralTable kPlural = make_slavic_plural_table();

class RussianVerbalizer : public VerbalizerBackend {
public:
  const char *code() const override { return "ru"; }
  const char *name() const override { return "Русский"; }
  const char *zero() const override { return "ноль"; }

  const char *hundred(std::size_t digit) const override {
    return kHundreds[digit];
  }

  TensUnitsWord tens_units(Gender gender, std::size_t tens_digit,
                           std::size_t units_digit) const override {
    const TensUnitsTable &table =
        gender == Gender::Fem ? kTensUnitsFem : kTensUnitsMasc;
    return table[tens_digit][units_digit];
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

  Gender unit_gender_for_scale(std::size_t scale_idx) const override {
    return scale_idx == 1 ? Gender::Fem : Gender::Masc;
  }
};

const RussianVerbalizer kRussianVerbalizer;
const RegisterVerbalizer kRussianRegistration{kRussianVerbalizer};

} // namespace
} // namespace verbose
