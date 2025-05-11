#include <Helper/ConstantEntry.hpp>

ConstantEntry::ConstantEntry(wgpu::StringView const& constantKey, double constantValue) {
    key = constantKey;
    value = constantValue;
}