#include <Helper/BlendState.hpp>

BlendState::BlendState(BlendComponent const& colorComponent, BlendComponent const& alphaComponent) {
	color = colorComponent;
	alpha = alphaComponent;
}
