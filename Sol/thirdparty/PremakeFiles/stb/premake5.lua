project "stb"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"stb_c_lexer.h",
		"stb_connected_components.h",
		"stb_divide.h",
		"stb_ds.h",
		"stb_dxt.h",
		"stb_easy_font.h",
		"stb_herringbone_wang_tile.h",
		"stb_hexwave.h",
		"stb_image.h",
		"stb_image_resize.h",
		"stb_image_write.h",
		"stb_include.h",
		"stb_leakcheck.h",
		"stb_rect_pack.h",
		"stb_sprintf.h",
		"stb_textedit.h",
		"stb_tilemap_editor.h",
		"stb_truetype.h",
		"stb_vorbis.c",
		"stb_voxel_render.h"
	}

	filter "system:linux"
		pic "On"

		systemversion "latest"
		
		files
		{
			"stb_c_lexer.h",
			"stb_connected_components.h",
			"stb_divide.h",
			"stb_ds.h",
			"stb_dxt.h",
			"stb_easy_font.h",
			"stb_herringbone_wang_tile.h",
			"stb_hexwave.h",
			"stb_image.h",
			"stb_image_resize.h",
			"stb_image_write.h",
			"stb_include.h",
			"stb_leakcheck.h",
			"stb_rect_pack.h",
			"stb_sprintf.h",
			"stb_textedit.h",
			"stb_tilemap_editor.h",
			"stb_truetype.h",
			"stb_vorbis.c",
			"stb_voxel_render.h"
		}

		defines
		{
			"_GLFW_X11"
		}

	filter "system:windows"
		systemversion "latest"

		files
		{
			"stb_c_lexer.h",
			"stb_connected_components.h",
			"stb_divide.h",
			"stb_ds.h",
			"stb_dxt.h",
			"stb_easy_font.h",
			"stb_herringbone_wang_tile.h",
			"stb_hexwave.h",
			"stb_image.h",
			"stb_image_resize.h",
			"stb_image_write.h",
			"stb_include.h",
			"stb_leakcheck.h",
			"stb_rect_pack.h",
			"stb_sprintf.h",
			"stb_textedit.h",
			"stb_tilemap_editor.h",
			"stb_truetype.h",
			"stb_vorbis.c",
			"stb_voxel_render.h"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
        symbols "off"