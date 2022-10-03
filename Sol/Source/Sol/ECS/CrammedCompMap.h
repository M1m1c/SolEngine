#pragma once
#include "ECS_Core.h"
#include "IComponent.h"
#include <cstdint>
#include <vector>
#include <unordered_map>
namespace Sol 
{
	//TODO have an array of items,
	//have an array of keys that assosiate with a index in item array,
	//needs to increase size to be the last element +1 of array
	//whenever we add it needs to be added at size index.
	//whenever we remove it needs to take last element and stuff it into psoition of removed elemetn and shorten size by -1.
	//Also wheever adding or removing we need to calculate the key to index and index to key mappings

	/*template <typename T>
	class Key {
	private:
		T value;
		DataType<T> type;
	};*/
	

	//template<Entity TKey, IComponent& TValue, size_t TCapacity = 0>
	template<class TValue, size_t TCapacity = 0>
	class CrammedCompMap
	{
	public:

		CrammedCompMap() :
			m_Capacity(TCapacity),
			m_Container(TValue, capacity)
		{

		}
		~CrammedCompMap(){}

		int& operator[](int index) { return m_Container[index]; }

		const size_t GetSize() { return m_Size; }
		const size_t GetCapacity() { return m_Capacity; }
		const TValue& GetWithKey(Entity key) { return m_Container[m_KeyToIndexMap[Key<key>]]; }
		const void Add(Entity key,TValue element)
		{
			assert((m_Size + 1) > m_Capacity && "Failed adding element! Exceeding capacity of container.");
			m_Container[m_Size] = element;
			m_KeyToIndexMap[Key<key>] = m_Size;
			m_IndexToKeyMap[m_Size] = Key<key>;
			m_NewSize = m_Size + 1;
		}
		const void Remove(Entity key)
		{
			//TODO assert that we can find element with key
			auto& last = m_Container[m_Size - 1];
			m_Container[m_KeyToIndexMap[Key<key>]] = last;
			m_Size -= 1;
			last = nullptr;
		}

		//const void Remove(size_t index){}

		const void Refresh() 
		{
			m_Size = m_NewSize;
		}

	private:
		size_t m_Size = 0;
		size_t m_NewSize = 0;
		size_t m_Capacity = 0;

		std::vector<TValue, TCapacity> m_Container;

		std::unordered_map<Entity, size_t> m_KeyToIndexMap;
		std::unordered_map<size_t, Entity> m_IndexToKeyMap;


	};

	
}