#include <Helper/TexelCopyBufferLayout.hpp>

TexelCopyBufferLayout::TexelCopyBufferLayout(uint32_t bytesPerRow, uint32_t rowsPerImage) {
	this->bytesPerRow = bytesPerRow;
	this->rowsPerImage = rowsPerImage;
	this->offset = 0;
}
