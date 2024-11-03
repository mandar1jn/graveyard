workspace "AssetExtractors"
	configurations {"Debug", "Release"}
	language "C++"
	location "../build"
	cppdialect "C++20"
	cdialect "C99"
	staticruntime "on"
	architecture "x64"
	floatingpoint "Strict"
	startproject "AWExtractor"

	filter "configurations:Debug"
		defines {"Debug"}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

include "../ExtractorCore"

group "Externals"
	project "ImGui"
		kind "StaticLib"
		libdirs {"../externals/GLFW"}
		links {"GLFW"}
		files {
			"../externals/imgui/backends/imgui_impl_glfw.cpp",
			"../externals/imgui/backends/imgui_impl_glfw.h",
			"../externals/imgui/backends/imgui_impl_opengl3.cpp",
			"../externals/imgui/backends/imgui_impl_opengl3.h",
			"../externals/imgui/*.h",
			"../externals/imgui/*.cpp"
		}
		includedirs {
			"../externals/imgui",
			"../externals/imgui/backends",

			"../externals/GLFW/include"
		}
	project "glad"
		kind "StaticLib"
		language "C"
		files {
			"../externals/glad/**/*.c",
			"../externals/glad/**/*.h"
		}
		includedirs {
			"../externals/glad/include",
		}

	include "./glfw.lua"

group "Extractors"
	include "../extractors/AWExtractor/premake5.lua"