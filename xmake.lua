package("wgpu-native-cpp")
    add_urls("https://github.com/eliemichel/WebGPU-Cpp.git")
    add_versions("2024.08.31", "02403c48c8c2684ba1138355d505a2f9f3d72a93")

    add_deps("wgpu-native")

    on_install(function (package)
        os.vcp("wgpu-native/webgpu.hpp", package:installdir("include", "wgpu-native"))
    end)
    
    on_test(function (package) assert(package:check_cxxsnippets([[ void test() { wgpu::Instance instance; } ]], { includes = "wgpu-native/webgpu.hpp" })) end)
package_end()

package("sdl2webgpu")
    set_homepage("https://github.com/eliemichel/sdl2webgpu.git")
    set_description("An extension for the SDL2 library for using WebGPU native.")
    set_license("MIT")

    add_urls("https://github.com/eliemichel/sdl2webgpu.git")
    add_versions("2024.06.21", "ed785c8e48bd178fd6392d3c540c0ac4746f008d")

    add_deps("cmake", "libsdl", "wgpu-native")

    on_install(function (package)
        local configs = {}
        table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:is_debug() and "Debug" or "Release"))
        table.insert(configs, "-DBUILD_SHARED_LIBS=" .. (package:config("shared") and "ON" or "OFF"))
        import("package.tools.cmake").install(package, configs, {packagedeps = {"libsdl", "wgpu-native"}})
        os.vcp("sdl2webgpu.h", package:installdir("include"))
    end)

    on_test(function (package)
        assert(package:has_cfuncs("SDL_GetWGPUSurface", {includes = "sdl2webgpu.h", configs = {defines = "SDL_MAIN_HANDLED"}}))
    end)
package_end()

add_requires("wgpu-native", "libsdl", "sdl2webgpu", "wgpu-native-cpp", "glm", "tinyobjloader", "stb", "snitch")

add_rules("mode.debug", "mode.release", "mode.coverage")
set_warnings("all", "error")
set_languages("c++20")
set_optimize("fastest")

add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})
add_rules("plugin.vsxmake.autoupdate")
target("wgpu-test")
    set_kind("binary")
    add_packages("wgpu-native", "libsdl", "sdl2webgpu", "wgpu-native-cpp", "glm", "tinyobjloader", "stb")

    set_warnings("error")

    add_files("src/*.cpp")

    add_headerfiles("inc/*.hpp")

    add_includedirs("inc")
    add_extrafiles("xmake.lua")
    add_extrafiles("resources/*")

    set_configdir("$(buildir)/$(plat)/$(arch)/$(mode)/resources")
    add_configfiles("resources/*")
target_end()

target("vector2-tests")
    set_kind("binary")

    set_default(false)
    add_packages("snitch")

    set_runargs("-v", "full", "*")

    add_files("tests/Vector2.test.cpp")
    add_files("src/Vector2.cpp")
    add_headerfiles("inc/Vector2.hpp")
    add_tests("tests-vector2")

    add_includedirs("inc")

    set_warnings("error")
target_end()