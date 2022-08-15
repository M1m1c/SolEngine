workspace "Sol"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Sol"
	location "Sol"
	kind "SharedLib"
	language "C++"

	targetdir ("binaries/" .. outputdir .. "/%{prj.name}/")
	objdir ("intermediate/" .. outputdir .. "/%{prj.name}/")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/Source",
		"%{prj.name}/thirdparty/spdlog/include;"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"SOL_PLATFORM_WINDOWS",
			"SOL_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../binaries/".. outputdir .."/Sandbox")
		}

	filter "configurations:Debug"
		defines "SOL_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "SOL_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "SOL_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

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
		"Sol/Source"
	}

	links
	{
		"Sol"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"SOL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "SOL_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "SOL_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "SOL_DIST"
		optimize "On"