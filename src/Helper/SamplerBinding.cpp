#include <Helper/SamplerBinding.hpp>

wgpu::BindGroupEntry CreateSamplerBinding(uint32_t binding, wgpu::Sampler& sampler) {
	wgpu::BindGroupEntry bindGroupEntry{};

	bindGroupEntry.binding = binding;
	bindGroupEntry.sampler = sampler;

	return bindGroupEntry;
}

void EditSamplerBinding(wgpu::BindGroupEntry& bindGroupEntry, uint32_t binding, wgpu::Sampler& sampler) {
	bindGroupEntry.binding = binding;

	bindGroupEntry.sampler = sampler;
}

void DisplaySamplerBinding(wgpu::BindGroupEntry const& bindGroupEntry) {
	std::cout << "- " << "Sampler binding:" << std::endl;
	std::cout << "\t- " << "binding: " << bindGroupEntry.binding << std::endl;
	std::cout << "\t- " << "sampler: " << bindGroupEntry.sampler << std::endl;
}
