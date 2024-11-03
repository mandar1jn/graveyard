project "GLFW"
	kind "StaticLib"
	language "C"

	files
	{
		"../externals/GLFW/include/GLFW/glfw3.h",
		"../externals/GLFW/include/GLFW/glfw3native.h",
		"../externals/GLFW/src/glfw_config.h",
		"../externals/GLFW/src/context.c",
		"../externals/GLFW/src/init.c",
		"../externals/GLFW/src/input.c",
		"../externals/GLFW/src/monitor.c",
		"../externals/GLFW/src/platform.c",
		"../externals/GLFW/src/vulkan.c",
		"../externals/GLFW/src/window.c",
		"../externals/GLFW/src/null_init.c",
		"../externals/GLFW/src/null_monitor.c",
		"../externals/GLFW/src/null_window.c",
		"../externals/GLFW/src/null_joystick.c"
	}
	filter "system:linux"
		pic "On"

		files
		{
			"../externals/GLFW/src/x11_init.c",
			"../externals/GLFW/src/x11_monitor.c",
			"../externals/GLFW/src/x11_window.c",
			"../externals/GLFW/src/xkb_unicode.c",
			"../externals/GLFW/src/posix_time.c",
			"../externals/GLFW/src/posix_thread.c",
			"../externals/GLFW/src/glx_context.c",
			"../externals/GLFW/src/egl_context.c",
			"../externals/GLFW/src/osmesa_context.c",
			"../externals/GLFW/src/linux_joystick.c"
		}

		defines
		{
			"_GLFW_X11"
		}

	filter "system:windows"

		files
		{
			"../externals/GLFW/src/win32_init.c",
			"../externals/GLFW/src/win32_joystick.c",
			"../externals/GLFW/src/win32_module.c",
			"../externals/GLFW/src/win32_monitor.c",
			"../externals/GLFW/src/win32_time.c",
			"../externals/GLFW/src/win32_thread.c",
			"../externals/GLFW/src/win32_window.c",
			"../externals/GLFW/src/wgl_context.c",
			"../externals/GLFW/src/egl_context.c",
			"../externals/GLFW/src/osmesa_context.c"
		}

		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}