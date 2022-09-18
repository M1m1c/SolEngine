#include "solpch.h"
#include "GalaxyMacros.h"

namespace GalaxyDraw 
{
	std::string GalaxyMacros::get_file_contents(const char* filename)
	{
		std::string targetPath = "../Sol/Source/GalaxyDraw/Shaders/";
		std::string targetPathDebug = "../Sol/Source/GalaxyDraw/Shaders/";
		targetPath.append(filename);

		std::ifstream in(targetPath, std::ios::binary);
		std::string contents="";

		if (in)
		{
			
			in.seekg(0, std::ios::end);
			contents.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();
			return (contents);
		}

		SOL_CORE_ASSERT(contents != "", "Filepath not found within >" + targetPath);
		throw (errno);
	}
}
