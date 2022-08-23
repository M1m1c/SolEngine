#include "ModelBuilder.h"

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
