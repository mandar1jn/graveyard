group "AWExtractor"
	project "AWExtractor"
		links {"ExtractorCore"}
		files {
			"src/**.cpp",
			"include/**.h"
		}
		includedirs {
			"include",

			-- external projects
			"../../externals/GLFW/include",
			"../../externals/imgui",
			"../../externals/imgui/backends",
			"../../externals/glad/include",
			"../../ExtractorCore/include",
			"../../externals/gli",
			"../../externals/gli/external"
		}

		filter "configurations:Debug"
			kind "ConsoleApp"

		filter "configurations:Release"
			kind "WindowedApp"