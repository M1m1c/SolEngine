#pragma once
#include "Sol/ECS/ECS_Core.h"
#include <cstdint>


namespace Sol 
{
	class Entity
	{
	public:
		Entity(EntityID id) { m_ID = id; }
		~Entity(){}
		EntityID GetID() { return m_ID; }
		bool IsPendingKill() { return m_bPendingKill; }
		void Destroy() { m_bPendingKill = true; }
	private:
		EntityID m_ID;
		bool m_bPendingKill = false;
	};
}

