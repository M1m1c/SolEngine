#include "solpch.h"

#include "ModelBuilder.h"

namespace GalaxyDraw {
	ModelBuilder::ModelBuilder()
	{

	}

	ModelBuilder::~ModelBuilder()
	{

	}

	ModelOld ModelBuilder::Build()
	{
		return ModelOld(path);
	}
}