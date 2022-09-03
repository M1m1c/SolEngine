#include "solpch.h"

#include "ModelBuilder.h"

namespace GalaxyDraw {
	ModelBuilder::ModelBuilder()
	{

	}

	ModelBuilder::~ModelBuilder()
	{

	}

	Model ModelBuilder::Build()
	{
		return Model(path);
	}
}