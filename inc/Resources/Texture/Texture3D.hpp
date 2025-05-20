#ifndef TEXTURE3D_HPP
#define TEXTURE3D_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <stdexcept>

#include <stb_image.h>

#include <Helper/Handles.hpp>
#include <Helper/Descriptors.hpp>

class Texture3D {


    private:
        Texture _texture;
        TextureView _textureView;
};

#endif // TEXTURE3D_HPP