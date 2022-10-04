#pragma once
#include <cstdint>
#include <vector>
#include <unordered_map>
namespace Sol 
{

	class ICrammedVecMap
	{
	public:
		virtual ~ICrammedVecMap() = default;
		virtual inline void DeleteKey(uint32_t key) = 0;
	};

	template<class TValue>
	class CrammedVecMap : ICrammedVecMap
	{
	public:

		CrammedVecMap(size_t capacity = 0) :
			m_Capacity(capacity),
			m_Collection(TValue, capacity)
		{

		}
		~CrammedVecMap(){}

		TValue& operator[](size_t index) { return m_Collection[index]; }

		inline const size_t GetSize() { return m_Size; }
		inline const size_t GetCapacity() { return m_Capacity; }

		//Gets element with provided key
		inline const TValue& GetWithKey(uint32_t key) 
		{
			assert(m_KeyToIndexMap.find(key) != m_KeyToIndexMap.end() && "Failed Retriving element! Non-existent element.");
			return m_Collection[m_KeyToIndexMap[Key<key>]]; 
		}

		//Adds new element with key to collection.
		//OBS! make sure to call Refresh() in order to update the size of collection when new elements are ready to be used.
		inline void Add(uint32_t key, TValue element)
		{
			size_t newSize = m_Size + 1;
			assert((newSize) > m_Capacity && "Failed adding element! Exceeding capacity of container.");
			assert(m_KeyToIndexMap.find(key) == m_KeyToIndexMap.end() && "Failed adding element! Component added to same entity more than once.");

			m_Collection[m_Size] = element;

			m_KeyToIndexMap[key] = m_Size;
			m_IndexToKeyMap[m_Size] = key;

			m_NewSize = newSize;
		}
		
		//Used to remove elements related to key
		inline void Remove(uint32_t key)
		{
			assert(m_KeyToIndexMap.find(key) != m_KeyToIndexMap.end() && "Failed removing element! Non-existent element.");

			size_t lastIndex = m_Size - 1;
			size_t removedIndex = m_KeyToIndexMap[key];
			size_t keyOfLastElement = m_IndexToKeyMap[lastIndex];

			m_Collection[removedIndex] = m_Collection[lastIndex];

			m_KeyToIndexMap[keyOfLastElement] = removedIndex;
			m_IndexToKeyMap[removedIndex] = keyOfLastElement;

			m_KeyToIndexMap.erase(key);
			m_IndexToKeyMap.erase(lastIndex);

			m_Size -= 1;
		}

		//Used to remove key and elements related to key from collection
		virtual inline void DeleteKey(uint32_t key) override
		{
			if (m_KeyToIndexMap.find(entity) != m_KeyToIndexMap.end())
			{
				Remove(key);
			}
		}

		//Used to increase size of index of colleciton when new elements have been initalised.
		inline void Refresh() 
		{
			m_Size = m_NewSize;
		}

	private:
		size_t m_Size = 0;
		size_t m_NewSize = 0;
		size_t m_Capacity = 0;

		std::vector<TValue> m_Collection;

		std::unordered_map<uint32_t, size_t> m_KeyToIndexMap;
		std::unordered_map<size_t, uint32_t> m_IndexToKeyMap;
	};

	
}