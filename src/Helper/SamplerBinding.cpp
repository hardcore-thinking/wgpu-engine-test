#include <Helper/SamplerBinding.hpp>

SamplerBinding::SamplerBinding(uint32_t samplerBinding, Sampler& samplerHandle) {
	binding = samplerBinding;
	sampler = (WGPUSampler)(&samplerHandle);
}

std::ostream& operator<<(std::ostream& out, SamplerBinding const& bindGroupEntry) {
	std::cout << "- " << "Sampler binding:" << std::endl;
	std::cout << "\t- " << "binding: " << bindGroupEntry.binding << std::endl;
	std::cout << "\t- " << "sampler: " << bindGroupEntry.sampler << std::endl;

	return out;
}
