project "ExtractorCore"
	kind "StaticLib"
	links {"ImGui", "GLFW", "glad"}
	files {
		"../ExtractorCore/**/*.cpp",
		"../ExtractorCore/**/*.h"
	}
	includedirs {
		"../ExtractorCore/include",

		-- externals includes
		"../externals/GLFW/include",
		"../externals/imgui",
		"../externals/imgui/backends",
		"../externals/glad/include",
	}