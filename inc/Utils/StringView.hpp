#ifndef STRINGVIEW_HPP
#define STRINGVIEW_HPP

#include <iostream>
#include <string_view>

#include <wgpu-native/webgpu.hpp>

namespace Utils {
	std::string_view StrViewRepr(wgpu::StringView const& view);
}

#endif // STRINGVIEW_HPP
