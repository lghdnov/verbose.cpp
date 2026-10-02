#pragma once

#include <array>
#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace verbose {

enum class Gender { Masc, Fem, Neut };

enum class PluralForm { One, Few, Many };

static_assert(static_cast<std::size_t>(PluralForm::One) == 0);
static_assert(static_cast<std::size_t>(PluralForm::Few) == 1);
static_assert(static_cast<std::size_t>(PluralForm::Many) == 2);

struct TensUnitsWord {
  const char *tens;
  const char *unit;
};

using TensUnitsTable = std::array<std::array<TensUnitsWord, 10>, 10>;

constexpr TensUnitsTable
make_tens_units_table(const std::array<const char *, 10> &units,
                      const std::array<const char *, 10> &teens,
                      const std::array<const char *, 10> &tens) {

  TensUnitsTable table{};

  for (std::size_t u = 0; u < 10; ++u)
    table[0][u] = {"", units[u]};

  for (std::size_t u = 0; u < 10; ++u)
    table[1][u] = {"", teens[u]};

  for (std::size_t t = 2; t < 10; ++t)
    for (std::size_t u = 0; u < 10; ++u)
      table[t][u] = {tens[t], units[u]};

  return table;
}

using PluralTable = std::array<std::array<PluralForm, 10>, 10>;

constexpr PluralTable make_slavic_plural_table() {
  PluralTable table{};

  for (auto &row : table)
    std::fill(row.begin(), row.end(), PluralForm::Many);

  for (std::size_t t = 0; t < 10; ++t) {
    if (t == 1)
      continue;
    table[t][1] = PluralForm::One;
    table[t][2] = table[t][3] = table[t][4] = PluralForm::Few;
  }

  return table;
}

class VerbalizerBackend {
public:
  virtual ~VerbalizerBackend() = default;

  virtual const char *code() const = 0;
  virtual const char *name() const = 0;
  virtual const char *zero() const = 0;
  virtual const char *hundred(std::size_t digit) const = 0;

  virtual TensUnitsWord tens_units(Gender gender, std::size_t tens_digit,
                                   std::size_t units_digit) const = 0;
  virtual const char *scale_form(std::size_t scale_idx,
                                 PluralForm form) const = 0;

  virtual PluralForm plural(std::size_t tens_digit,
                            std::size_t units_digit) const = 0;
  virtual Gender unit_gender_for_scale(std::size_t scale_idx) const = 0;
};

std::string verbalize(const VerbalizerBackend &lang, std::uint64_t n);
void verbalize_number(const VerbalizerBackend &lang, std::uint64_t n,
                      std::string &out);

class VerbalizerRegistry {
public:
  static VerbalizerRegistry &instance();

  void add(const VerbalizerBackend *verbalizer);
  const VerbalizerBackend *get(std::string_view code) const;
  std::vector<std::string_view> codes() const;
  std::string codes_string() const;

private:
  VerbalizerRegistry() = default;

  std::map<std::string_view, const VerbalizerBackend *, std::less<>> map_;
};

struct RegisterVerbalizer {
  explicit RegisterVerbalizer(const VerbalizerBackend &verbalizer) {
    VerbalizerRegistry::instance().add(&verbalizer);
  }
};

} // namespace verbose
