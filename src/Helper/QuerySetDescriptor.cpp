#include <Helper/QuerySetDescriptor.hpp>

QuerySetDescriptor::QuerySetDescriptor(uint32_t queryCount, wgpu::QueryType queryType) {
	count = queryCount;
	type = queryType;
	nextInChain = nullptr;
}
