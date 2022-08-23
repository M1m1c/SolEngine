#ifndef MODEL_BUILDER_H
#define MODEL_BUILDER_H

#include "IBuildable.h"
#include "Model.h"

class ModelBuilder : public IBuildable<Model>
{
public:
	BUILDABLE_PROPERTY(ModelBuilder, std::string, path)

	ModelBuilder();
	~ModelBuilder();

	Model Build();
};

#endif