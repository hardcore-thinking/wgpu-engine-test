add_rules("mode.debug", "mode.release", "mode.coverage")
set_warnings("all", "error")
set_languages("cxx17")
set_optimize("fastest")

add_requireconfs("wgpu-native", "**.wgpu-native", { version = "v24.0.0+1", override = true })

package("wgpu-native-cpp")
    set_kind("library", { headeronly = true })

    set_homepage("https://github.com/eliemichel/WebGPU-Cpp.git")
    set_description("CMake files that provide WebGPU for native and web development, easy-to-integrate and unified across implementations.")
    set_license("MIT")
    
    set_urls("https://github.com/eliemichel/WebGPU-Cpp.git")
    add_versions("2025.06.04", "f72670b327c394a0197536ba8c389ce08ecf560d")
    
    add_deps("wgpu-native", "python 3.13.2")

    on_install(function (package)
        -- local headerPath = path.join(package:dep("wgpu-native"):installdir("include/webgpu"), "webgpu.h")
        -- print(headerPath)
        -- assert(os.exists(headerPath))
        -- os.execv("python generate.py", { "--use-init-macros", "-u", headerPath, "--template", "webgpu.template.hpp" })
        os.vcp("wgpu-native/webgpu.hpp", package:installdir("include", "wgpu-native"))
    end)
    
    on_test(function (package)
        assert(package:check_cxxsnippets([[ void test() { wgpu::Instance instance; } ]], { includes = "wgpu-native/webgpu.hpp", configs = { languages = "c++17" } }))
    end)
package_end()

set_languages("cxx20")
add_requires("sdl2webgpu", "wgpu-native-cpp", "tinyobjloader", "stb")
add_requires("wgpu-native 2025.06.04")
add_requires("libsdl2", { configs = { wayland = true, shared = true } })
add_requireconfs("sdl2webgpu.libsdl2", { configs = { shared = true } })
-- add_requires("imgui", { configs = { sdl2 = true, webgpu = true } })

add_rules("plugin.compile_commands.autoupdate", { outputdir = ".vscode" })
target("wgpu-test")
    set_kind("binary")

    if is_plat("linux") then end -- TODO
    if is_plat("windows") then end -- TODO
    if is_plat("macosx") then end -- TODO

    if is_mode("debug") then
        add_cxxflags("-fsanitize=address,undefined", {force = true})
        add_ldflags("-fsanitize=address,undefined", {force = true})
        add_cxxflags("-fno-omit-frame-pointer", {force = true})
    end

    add_packages("wgpu-native", "libsdl2", "sdl2webgpu") 
    add_packages("wgpu-native-cpp", "tinyobjloader", "stb")
    add_packages("imgui")

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

    set_rundir("./")
    add_installfiles("resources/**")
target_end()
