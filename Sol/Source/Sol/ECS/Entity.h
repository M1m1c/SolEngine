#pragma once
#include "Sol/Core/Core.h"
#include "IComponent.h"
#include <cstdint>
#include <unordered_set>
namespace Sol 
{
	class Entity
	{
	public:
		Entity();
		~Entity();


		//IComponent GetComponent(IComponent type) const;

	private:
		uint32_t m_EntityID;
		std::unordered_set<s_ptr<IComponent>> m_Components;
	};
}

