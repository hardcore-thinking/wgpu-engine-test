#include <Helper/TextureBinding.hpp>

TextureBinding::TextureBinding(uint32_t textureBinding, TextureView& textureViewHandle) {	
	binding = textureBinding;
	textureView = textureViewHandle.Handle();
}

std::ostream& operator<<(std::ostream& out, TextureBinding const& bindGroupEntry) {
	out << "- " << "Texture binding:" << std::endl;
	out << "\t- " << "binding: " << bindGroupEntry.binding << std::endl;
	out << "\t- " << "textureView: " << bindGroupEntry.textureView << std::endl;

	return out;
}
