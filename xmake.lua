add_rules("mode.debug", "mode.release", "mode.coverage")
set_warnings("all", "error")
set_languages("cxx17")
set_optimize("fastest")

package("wgpu-native-cpp")
    set_homepage("https://github.com/eliemichel/WebGPU-Cpp.git")
    set_description("CMake files that provide WebGPU for native and web development, easy-to-integrate and unified across implementations.")
    set_license("MIT")
    
    set_urls("https://github.com/eliemichel/WebGPU-Cpp.git")
    add_versions("2025.05.08", "3938273103e69a5af3288196deccc48774d955f3")
    
    add_deps("wgpu-native")

    on_install(function (package)
        os.vcp("wgpu-native/webgpu.hpp", package:installdir("include", "wgpu-native"))
        os.vcp("wgpu-native/wgpu.h", package:installdir("include", "wgpu-native", "webgpu"))
    end)
    
    on_test(function (package)
        assert(package:check_cxxsnippets([[ void test() { wgpu::Instance instance; } ]], { includes = "wgpu-native/webgpu.hpp", configs = { languages = "c++17" } }))
    end)
package_end()

package("sdl2webgpu")
    set_homepage("https://github.com/eliemichel/sdl2webgpu.git")
    set_description("An extension for the SDL2 library for using WebGPU native.")
    set_license("MIT")

    add_urls("https://github.com/eliemichel/sdl2webgpu.git")
    add_versions("2025-04-18", "eda8bedcb78a755f42c5efa00b732c62e89c23cb")
    --add_versions("2024-10-16", "860a0444aff94d82e9cd96ea9085a7e4fefce55d")
    
    add_deps("libsdl2", "wgpu-native")

    on_install(function (package)
        io.writefile("xmake.lua", [[
            add_rules("mode.debug", "mode.release")
            set_languages("cxx17")
            add_requires("libsdl2", "wgpu-native")
            target("sdl2wgpu")
                add_packages("libsdl2", "wgpu-native")
                set_kind("$(kind)")
                add_files("sdl2webgpu.c")
                add_headerfiles("sdl2webgpu.h")
            target_end()
        ]])
        os.vcp("sdl2webgpu.h", package:installdir("include"))
        
        import("package.tools.xmake").install(package)
    end)

    on_test(function (package)
        assert(package:has_cfuncs("SDL_GetWGPUSurface", {includes = "sdl2webgpu.h", configs = {defines = "SDL_MAIN_HANDLED"}}))
    end)
package_end()

set_languages("cxx20")
add_requires("sdl2webgpu", "wgpu-native-cpp", "glm", "tinyobjloader", "stb", "snitch")
add_requires("wgpu-native v24.0.0+1", "libsdl2")

add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})
add_rules("plugin.vsxmake.autoupdate")
target("wgpu-test")
    set_kind("binary")
    add_packages("wgpu-native", "libsdl2", "sdl2webgpu", "wgpu-native-cpp", "glm", "tinyobjloader", "stb")

    set_warnings("error")

    add_files("src/*.cpp")
    add_files("src/Math/*.cpp")
    add_files("src/Helper/*.cpp")
    add_files("src/Resources/Geometry/*.cpp")
    add_files("src/Resources/Texture/*.cpp")

    add_headerfiles("inc/*.hpp")
    add_headerfiles("inc/Math/*.hpp")
    add_headerfiles("inc/Helper/*.hpp")
    add_headerfiles("inc/Resources/Geometry/*.hpp")
    add_headerfiles("inc/Resources/Texture/*.hpp")

    add_includedirs("inc")
    add_extrafiles("xmake.lua")
    add_extrafiles("resources/**")

    set_rundir(".")
    add_installfiles("resources/**")
target_end()

target("vector2-tests")
    set_kind("binary")

    set_default(false)
    add_packages("snitch")

    set_runargs("-v", "full", "*")

    add_files("tests/Vector2.test.cpp")
    add_files("src/Math/Vector2.cpp")
    add_headerfiles("inc/Math/Vector2.hpp")
    add_tests("tests-vector2")

    add_includedirs("inc")

    set_warnings("error")
target_end()