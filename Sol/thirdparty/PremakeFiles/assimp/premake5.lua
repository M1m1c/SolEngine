project "assimp"
  kind "SharedLib"
  language "C++"
  staticruntime "off"

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

  files {
    "include/**",
    'assimp/contrib/unzip/**',
    'assimp/contrib/irrXML/**',
    'assimp/contrib/zlib/*',
    'assimp/code/Common/**',
    'assimp/code/PostProcessing/**',
    'assimp/code/Material/**',
    'assimp/code/CApi/**',
    'assimp/code/Collada/**',
    'assimp/code/Obj/**',
    'assimp/code/FBX/**',
    'assimp/code/Assbin/**'
}

  includedirs {
    "include",
    "include/assimp",
    '_config_headers/',
    '_config_headers/assimp/',
    'include/assimp',
    'assimp/',
    'assimp/contrib/',
    'assimp/contrib/irrXML/',
    'assimp/contrib/unzip/',
    'assimp/contrib/rapidjson/include/',
    'assimp/code'
}


   filter "system:windows"
      systemversion "latest"

   filter  "configurations:Debug"
       runtime "Debug"
       symbols "on"

   filter  "configurations:Release"
       runtime "Release"
       optimize "on"