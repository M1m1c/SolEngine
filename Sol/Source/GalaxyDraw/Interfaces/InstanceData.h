#pragma once

#include<glm/glm.hpp>


namespace GalaxyDraw
{
	//!VERY IMPORTANT!
	// The order of initaliseation needs to match the location order in the shader,
	// otherwise the columns will be missaligned.
	struct InstanceData
	{
		InstanceData() :
			m_EntityID(-1),
			m_MeshColor(),
			m_EntityTransform(),
			m_MeshTransform()
			//	m_MeshPosition(),
		{};

		int m_EntityID;
		glm::vec4 m_MeshColor;
		glm::mat4 m_EntityTransform;
		glm::mat4 m_MeshTransform;
		//glm::vec3 m_MeshPosition;
	};
}