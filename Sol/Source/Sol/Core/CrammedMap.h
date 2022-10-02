#pragma once
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

	template <typename T>
	class Key {
	private:
		T value;
		DataType<T> type;
	};

	template<typename TKey,typename TValue, size_t TCapacity = 0>
	class CrammedMap
	{
	public:
		CrammedMap() :
			m_Capacity(TCapacity),
			m_Container(TValue, capacity)
		{

		}
		~CrammedMap(){}

	private:
		size_t m_Size;
		size_t m_Capacity;

		std::vector<TValue, TCapacity> m_Container;

		std::unordered_map<Key<TKey>, size_t> m_KeyToIndexMap;
		std::unordered_map<size_t, Key<TKey>> m_IndexToKeyMap;
	};

	
}