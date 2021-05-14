workspace "Paperback"
	architecture "x64"

	configurations
	{
		"Debug", -- Debug Mode
		"Release", -- Release Mode
		"Dist" -- Distribution Mode
	}

-- Generates something like "Debug-Windows-x64"
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


-- SHIFT THIS WHOLE CHUNK INTO ANOTHER .LUA FILE WHEN THERE ARE MORE EXTERNAL LIBS
-- Create a struct called IncludeDir
IncludeDir = {}
-- Set compiler include directory inside the struct
IncludeDir["GLFW"] = "Paperback/vendor/GLFW/include"
-- Includes the premake5.lua inside "Paperback\Paperback\vendor\GLFW"
include "Paperback/vendor/GLFW"



project "Paperback"
	location "Paperback"
	kind "SharedLib" -- Generates DLL
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	-- Precompiled Headers
	pchheader "PaperbackPCH.h"
	pchsource "Paperback/src/PaperbackPCH.cpp"

	-- Include all .h and .cpp files in Paperback/src/
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	-- Include paths for all external libraries
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	-- Add to Project references
	links
	{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PAPERBACK_PLATFORM_WINDOWS",
			"PAPERBACK_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "PAPERBACK_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PAPERBACK_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "PAPERBACK_DIST"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp" -- Generates exe
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	-- Include all .h and .cpp files in Sandbox/src/
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	-- Include paths for all external libraries
	includedirs
	{
		"Paperback/vendor/spdlog/include",
		"Paperback/src",
		"Sandbox/src"
	}

	-- Add to Project references
	links
	{
		"Paperback"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PAPERBACK_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "PAPERBACK_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PAPERBACK_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "PAPERBACK_DIST"
		optimize "On"