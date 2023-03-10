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
IncludeDir ["assimp"] = "Sol/thirdparty/assimp/include"
IncludeDir ["entt"] = "Sol/thirdparty/entt"
IncludeDir ["yaml_cpp"] = "Sol/thirdparty/yaml-cpp/include"

group "Dependencies"
	include "Sol/thirdparty/GLFW"
	include "Sol/thirdparty/Glad"
	include "Sol/thirdparty/imgui"
	include "Sol/thirdparty/assimp"
	include "Sol/thirdparty/stb"
	include "Sol/thirdparty/yaml-cpp"
group ""

project "Sol"
	location "Sol"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("binaries/" .. outputdir .. "/%{prj.name}/")
	objdir ("intermediate/" .. outputdir .. "/%{prj.name}/")

	pchheader"solpch.h"
	pchsource "Sol/Source/solpch.cpp"
	
	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
		"%{prj.name}/thirdparty/glm/glm/**.hpp",
		"%{prj.name}/thirdparty/glm/glm/**.inl",
		"%{prj.name}/thirdparty/entt/**.hpp",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"YAML_CPP_STATIC_DEFINE"
	}

	includedirs
	{
		"%{prj.name}/Source",
		"%{prj.name}/thirdparty/spdlog/include;",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.assimp}/include",
		"%{IncludeDir.stb}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}"
	}
	

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"stb",
		"assimp",
		"yaml-cpp",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"
		


		defines
		{
			"SOL_PLATFORM_WINDOWS",
			"SOL_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} ../Sol/thirdparty/assimp/lib/*.dll \"../binaries/".. outputdir .."/Sandbox\""),
			("{COPY} ../Sol/thirdparty/assimp/lib/*.dll \"../binaries/".. outputdir .."/Sol-Editor\"")
		}

	filter "configurations:Debug"
		defines "SOL_DEBUG"
		runtime "Debug"
		symbols "on"
		buildoptions "/MDd"

	filter "configurations:Release"
		defines "SOL_RELEASE"
		runtime "Release"
		optimize "on"
		buildoptions "/MD"

	filter "configurations:Dist"
		defines "SOL_DIST"
		runtime "Release"
		optimize "on"
		buildoptions "/MD"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"%{IncludeDir.ImGui}",
		"Sol/Source",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"Sol",
		"ImGui"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"SOL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "SOL_DEBUG"
		symbols "on"
		buildoptions "/MDd"

	filter "configurations:Release"
		defines "SOL_RELEASE"
		optimize "on"
		buildoptions "/MD"

	filter "configurations:Dist"
		defines "SOL_DIST"
		optimize "on"
		buildoptions "/MD"

project "Sol-Editor"
	location "Sol-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"%{IncludeDir.ImGui}",
		"Sol/Source",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"Sol",
		"ImGui"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"SOL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "SOL_DEBUG"
		symbols "on"
		buildoptions "/MDd"

	filter "configurations:Release"
		defines "SOL_RELEASE"
		optimize "on"
		buildoptions "/MD"

	filter "configurations:Dist"
		defines "SOL_DIST"
		optimize "on"
		buildoptions "/MD"