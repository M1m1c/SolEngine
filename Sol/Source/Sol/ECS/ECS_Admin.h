#pragma once
#include "Sol/Core/Core.h"

namespace Sol 
{
	static class ECS_Admin
	{
	public:
		ECS_Admin();
		~ECS_Admin();

		//TODO this needs to keep track of all enteties and all components,
		// or have handeler classes that it can refrence that does.
		//what is important is that the components need to be stored and created by type so that they are adjacent in memory.
		//This is very important for performance, since we will have to update all components and enteties each frame,
		// and we want the path to get to them to be as smooth as possible with virtually no checks.
		//Looking into using either unorderd_Set or unorderd_map for storing these, 
		// since they both seem to store things next to eachother in memory.
		// Whenever we create or delete a component or entity we need to go through here, 
		// since this is what will create , hold refereces to and destory all such things
	private:

	};
}
