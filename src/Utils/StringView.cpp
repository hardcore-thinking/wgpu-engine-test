#include <Utils/StringView.hpp>

namespace Utils {
	std::string_view StrViewRepr(wgpu::StringView const& view) {
		return std::string_view(view.data, view.length);
	}
}
