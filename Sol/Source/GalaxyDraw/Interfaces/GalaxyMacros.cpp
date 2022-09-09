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

		if (in)
		{
			std::string contents;
			in.seekg(0, std::ios::end);
			contents.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();
			return (contents);
		}

		//TODO Luds, I seem to need the Sol/Log.h file to run the engine's logs, so as a hacky fix, I'm using std::cout for now
		std::cout << "Filepath not found within >" << targetPathDebug;

		throw (errno);
	}
}
