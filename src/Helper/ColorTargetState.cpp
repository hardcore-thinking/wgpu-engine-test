#include <Helper/ColorTargetState.hpp>

ColorTargetState::ColorTargetState(Adapter& adapter, CompatibleSurface& compatibleSurface, BlendState const& blendState) {
	wgpu::SurfaceCapabilities surfaceCapabilities;
	compatibleSurface->getCapabilities(adapter.Handle(), &surfaceCapabilities);

	blend = &blendState;
	format = surfaceCapabilities.formats[0];
	writeMask = wgpu::ColorWriteMask::All;
	nextInChain = nullptr;
}
