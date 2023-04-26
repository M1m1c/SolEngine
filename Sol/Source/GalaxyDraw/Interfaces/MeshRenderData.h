#pragma once

#include "InstanceData.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "Mesh.h"
#include "Texture.h"

#include "Sol/SolDefines.h"
#include "Sol/Core/KeyedVector.h"

namespace GalaxyDraw
{
	//for each unique mesh one of these structs are created,
	//it contains the relevant buffers and data for rendering instances of said mesh.
	struct MeshRenderData
	{
		std::string Name;
		std::shared_ptr<Mesh> m_Mesh;
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::shared_ptr<InstanceBuffer> m_InstanceBuffer;

		KeyedVector<EntityID, InstanceData> m_Instances;

		Vertex* VertexBufferBase = nullptr;
		Vertex* VertexBufferPtr = nullptr;
		InstanceData* InstanceBufferBase = nullptr;
		InstanceData* InstanceBufferPtr = nullptr;
	};
}