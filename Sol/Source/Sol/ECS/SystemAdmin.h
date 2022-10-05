#pragma once
#include "ECS_Core.h"
#include "ISystem.h"

namespace Sol 
{
	class SystemAdmin
	{
	public:
		SystemAdmin(){};
		~SystemAdmin(){};

		template<typename T>
		s_ptr<T> RegisterSystem();
		

		// Set the signature for this system
		template<typename T>
		inline void SetSignature(CompSignature signature);
	

		// Erase a destroyed entity from all system lists
		// mEntities is a set so no check needed
		inline void EntityDestroyed(EntityID entity);
		
		// Notify each system that an entity's signature changed
		inline void EntitySignatureChanged(EntityID entity, CompSignature entitySignature);
		
	private:
		// Map from system type string pointer to a signature
		std::unordered_map<const char*, CompSignature> m_Signatures;

		// Map from system type string pointer to a system pointer
		std::unordered_map<const char*, std::shared_ptr<ISystem>> m_Systems;
	};
}