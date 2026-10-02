#include "verbalizer/verbalizer.hpp"

namespace verbose {

VerbalizerRegistry &VerbalizerRegistry::instance() {
  static VerbalizerRegistry registry;
  return registry;
}

void VerbalizerRegistry::add(const VerbalizerBackend *verbalizer) {
  map_.emplace(verbalizer->code(), verbalizer);
}

const VerbalizerBackend *VerbalizerRegistry::get(std::string_view code) const {
  const auto it = map_.find(code);
  return it == map_.end() ? nullptr : it->second;
}

std::vector<std::string_view> VerbalizerRegistry::codes() const {
  std::vector<std::string_view> result;
  result.reserve(map_.size());
  for (const auto &entry : map_) {
    result.push_back(entry.first);
  }
  return result;
}

std::string VerbalizerRegistry::codes_string() const {
  std::string result;
  for (const auto &entry : map_) {
    if (!result.empty()) {
      result.append(", ");
    }
    result.append(entry.first);
  }
  return result;
}

} // namespace verbose
