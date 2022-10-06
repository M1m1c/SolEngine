#include "solpch.h"
#include "SystemAdmin.h"

namespace Sol 
{
	template<typename T>
	s_ptr<T> SystemAdmin::RegisterSystem()
	{
		const char* typeName = typeid(T).name();
		assert(mSystems.find(typeName) == mSystems.end() && "Attempted to register system type more than once.");
		auto system = std::make_shared<T>();
		mSystems.insert({ typeName, system });
		return system;
	}

	template<typename T>
	inline void SystemAdmin::SetSignature(CompSignature signature)
	{
		const char* typeName = typeid(T).name();
		assert(mSystems.find(typeName) != mSystems.end() && "System used before being registered.");
		mSignatures.insert({ typeName, signature });
	}

	inline void SystemAdmin::EntityDestroyed(EntityID entity)
	{

		for (auto const& pair : m_Systems)
		{
			auto const& system = pair.second;

			system->RemoveEntitity(entity);
		}
	}

	inline void SystemAdmin::EntitySignatureChanged(EntityID entity, CompSignature entitySignature)
	{
		for (auto const& pair : m_Systems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = m_Signatures[type];

			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->AddEntitity(entity);
				//system->m_Entities.insert(entity);
			}
			else
			{
				system->RemoveEntitity(entity);
				//system->m_Entities.erase(entity);
			}
		}
	}
}