// Port of the Rust unit tests from en.rs, ru.rs and mod.rs.
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

#include "verbalizer/verbalizer.hpp"

using verbose::VerbalizerRegistry;
using verbose::verbalize;

namespace {

int failures = 0;

void check_eq(const std::string& actual, const std::string& expected, const char* what) {
    if (actual != expected) {
        std::cerr << "FAIL " << what << "\n  expected: \"" << expected
                  << "\"\n  actual:   \"" << actual << "\"\n";
        ++failures;
    }
}

void check(bool ok, const char* what) {
    if (!ok) {
        std::cerr << "FAIL " << what << "\n";
        ++failures;
    }
}

void test_english() {
    const VerbalizerRegistry& r = VerbalizerRegistry::instance();
    const auto* en = r.get("en");
    check(en != nullptr, "english registered");

    check_eq(verbalize(*en, 0), "zero", "en zero");
    check_eq(verbalize(*en, 1), "one", "en 1");
    check_eq(verbalize(*en, 5), "five", "en 5");
    check_eq(verbalize(*en, 10), "ten", "en 10");
    check_eq(verbalize(*en, 21), "twenty one", "en 21");
    check_eq(verbalize(*en, 99), "ninety nine", "en 99");

    check_eq(verbalize(*en, 100), "one hundred", "en 100");
    check_eq(verbalize(*en, 256), "two hundred fifty six", "en 256");

    check_eq(verbalize(*en, 1000), "one thousand", "en 1000");
    check_eq(verbalize(*en, 2000), "two thousand", "en 2000");
    check_eq(verbalize(*en, 5000), "five thousand", "en 5000");

    check_eq(verbalize(*en, 1'000'000), "one million", "en 1e6");
    check_eq(verbalize(*en, 2'000'000), "two million", "en 2e6");

    check_eq(verbalize(*en, 999'000'000'000'000), "nine hundred ninety nine trillion",
             "en 999e12");
    check_eq(verbalize(*en, 1'000'000'001), "one billion one", "en 1e9+1");

    check_eq(verbalize(*en, 200), "two hundred", "en 200");
    check_eq(verbalize(*en, 101), "one hundred one", "en 101");
    check_eq(verbalize(*en, 111), "one hundred eleven", "en 111");
    check_eq(verbalize(*en, 1001), "one thousand one", "en 1001");
}

void test_russian() {
    const VerbalizerRegistry& r = VerbalizerRegistry::instance();
    const auto* ru = r.get("ru");
    check(ru != nullptr, "russian registered");

    check_eq(verbalize(*ru, 0), "ноль", "ru zero");
    check_eq(verbalize(*ru, 1), "один", "ru 1");
    check_eq(verbalize(*ru, 5), "пять", "ru 5");
    check_eq(verbalize(*ru, 10), "десять", "ru 10");
    check_eq(verbalize(*ru, 21), "двадцать один", "ru 21");
    check_eq(verbalize(*ru, 99), "девяносто девять", "ru 99");

    check_eq(verbalize(*ru, 100), "сто", "ru 100");
    check_eq(verbalize(*ru, 256), "двести пятьдесят шесть", "ru 256");

    check_eq(verbalize(*ru, 1000), "одна тысяча", "ru 1000");
    check_eq(verbalize(*ru, 2000), "две тысячи", "ru 2000");
    check_eq(verbalize(*ru, 5000), "пять тысяч", "ru 5000");

    check_eq(verbalize(*ru, 1'000'000), "один миллион", "ru 1e6");
    check_eq(verbalize(*ru, 2'000'000), "два миллиона", "ru 2e6");

    check_eq(verbalize(*ru, 999'000'000'000'000), "девятьсот девяносто девять триллионов",
             "ru 999e12");
    check_eq(verbalize(*ru, 999'000'000'000'001),
             "девятьсот девяносто девять триллионов один", "ru 999e12+1");

    bool threw = false;
    try {
        verbalize(*ru, 1'000'000'000'000'000);
    } catch (const verbose::VerbalizeError&) {
        threw = true;
    }
    check(threw, "ru max+1 throws");

    check_eq(verbalize(*ru, 200), "двести", "ru 200");
    check_eq(verbalize(*ru, 101), "сто один", "ru 101");
    check_eq(verbalize(*ru, 111), "сто одиннадцать", "ru 111");
    check_eq(verbalize(*ru, 1001), "одна тысяча один", "ru 1001");
}

void test_registry() {
    const VerbalizerRegistry& r = VerbalizerRegistry::instance();
    check(r.get("ru") != nullptr, "registry get ru");
    check(r.get("nonexistent") == nullptr, "registry get nonexistent");

    check(!r.codes().empty(), "registry codes non-empty");
    check(!r.codes_string().empty(), "registry codes_string non-empty");
    check_eq(r.codes_string(), "en, ru", "registry codes sorted and joined");
}

void test_error() {
    const VerbalizerRegistry& r = VerbalizerRegistry::instance();
    const auto* en = r.get("en");
    try {
        verbalize(*en, 1'000'000'000'000'000);
        check(false, "max throws");
    } catch (const verbose::VerbalizeError& e) {
        check_eq(e.what(),
                 "Number 1000000000000000 exceeds maximum supported value 1000000000000000",
                 "error message");
        check(e.number() == 1'000'000'000'000'000, "error carries number");
        check(e.max() == 1'000'000'000'000'000, "error carries max");
    }
}

}  // namespace

int main() {
    test_english();
    test_russian();
    test_registry();
    test_error();

    if (failures != 0) {
        std::cerr << failures << " test(s) failed\n";
        return 1;
    }
    std::cout << "all tests passed\n";
    return 0;
}
