#ifndef MODEL_BUILDER_H
#define MODEL_BUILDER_H

#include "IBuildable.h"
#include "ModelOld.h"
namespace GalaxyDraw 
{
	class ModelBuilder : public IBuildable<ModelOld>
	{
	public:
		BUILDABLE_PROPERTY(ModelBuilder, std::string, path)

			ModelBuilder();
		~ModelBuilder();

		ModelOld Build();
	};
}
#endif