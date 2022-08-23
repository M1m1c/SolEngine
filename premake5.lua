workspace "Sol"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir ={}
IncludeDir ["GLFW"] = "Sol/thirdparty/GLFW/include"
IncludeDir ["Glad"] = "Sol/thirdparty/Glad/include"
IncludeDir ["ImGui"] = "Sol/thirdparty/imgui"
IncludeDir ["stb"] = "Sol/thirdparty/stb"
IncludeDir ["glm"] = "Sol/thirdparty/glm"

group "Dependencies"
	include "Sol/thirdparty/GLFW"
	include "Sol/thirdparty/Glad"
	include "Sol/thirdparty/imgui"
	include "Sol/thirdparty/stb"
group ""

project "Sol"
	location "Sol"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("binaries/" .. outputdir .. "/%{prj.name}/")
	objdir ("intermediate/" .. outputdir .. "/%{prj.name}/")

	pchheader"solpch.h"
	pchsource "Sol/Source/solpch.cpp"

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
		"%{prj.name}/thirdparty/glm/glm/**.hpp",
		"%{prj.name}/thirdparty/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/Source",
		"%{prj.name}/thirdparty/spdlog/include;",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"stb",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		


		defines
		{
			"SOL_PLATFORM_WINDOWS",
			"SOL_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../binaries/".. outputdir .."/Sandbox\"")
		}

	filter "configurations:Debug"
		defines "SOL_DEBUG"
		runtime "Debug"
		symbols "On"
		buildoptions "/MDd"

	filter "configurations:Release"
		defines "SOL_RELEASE"
		runtime "Release"
		optimize "On"
		buildoptions "/MD"

	filter "configurations:Dist"
		defines "SOL_DIST"
		runtime "Release"
		optimize "On"
		buildoptions "/MD"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("binaries/" .. outputdir .. "/%{prj.name}/")
	objdir ("intermediate/" .. outputdir .. "/%{prj.name}/")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{
		"Sol/thirdparty/spdlog/include;",
		"Sol/Source",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Sol"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"SOL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "SOL_DEBUG"
		symbols "On"
		buildoptions "/MDd"

	filter "configurations:Release"
		defines "SOL_RELEASE"
		optimize "On"
		buildoptions "/MD"

	filter "configurations:Dist"
		defines "SOL_DIST"
		optimize "On"
		buildoptions "/MD"