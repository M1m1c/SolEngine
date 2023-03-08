#include <map>
#include <vector>

template <typename Key, typename T>
class KeyedVector {
public:
	// Constructors and assignment operators
	KeyedVector() = default;
	KeyedVector(const KeyedVector& other) = default;
	KeyedVector& operator=(const KeyedVector& other) = default;
	KeyedVector(KeyedVector&& other) = default;
	KeyedVector& operator=(KeyedVector&& other) = default;

	T* data() { return m_Vector.data(); }
	const T* data() const { return m_Vector.data(); }

	// Element access
	T& Get(const Key& key)
	{
		auto it = m_KeyToIndex.find(key);
		if (it != m_KeyToIndex.end()) {
			// If the key is already in the map, return the element at the corresponding index
			return m_Vector[it->second];
		}
		else {
			// If the key is not in the map, throw an exception
			throw std::out_of_range("Key not found in KeyedVector");
		}
	}

	const T& Get(const Key& key) const {
		// Const version of the above operator
		auto it = m_KeyToIndex.find(key);
		if (it != m_KeyToIndex.end()) {
			return m_Vector[it->second];
		}
		else {
			throw std::out_of_range("Key not found in KeyedVector");
		}
	}

	T& operator[](size_t index) {
		// Return the element at the specified index
		return m_Vector[index];
	}

	const T& operator[](size_t index) const {
		// Const version of the above operator
		return m_Vector[index];
	}


	bool Exists(const Key& key)
	{
		auto it = m_KeyToIndex.find(key);
		if (it != m_KeyToIndex.end())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	// Iterators
	using iterator = typename std::vector<T>::iterator;
	using const_iterator = typename std::vector<T>::const_iterator;
	iterator begin() { return m_Vector.begin(); }
	const_iterator begin() const { return m_Vector.begin(); }
	iterator end() { return m_Vector.end(); }
	const_iterator end() const { return m_Vector.end(); }

	// Capacity
	size_t size() const { return m_Vector.size(); }
	bool empty() const { return m_Vector.empty(); }
	void reserve(size_t n) { m_Vector.reserve(n); }

	// Modifiers
	void clear() {
		m_Vector.clear();
		m_KeyToIndex.clear();
	}

	void push_back(const Key& key, const T& value) {
		m_Vector.push_back(value);
		size_t index = m_Vector.size() - 1;
		m_KeyToIndex[key] = index;
	}

	void push_back(const Key& key, T&& value) {
		m_Vector.push_back(std::move(value));
		size_t index = m_Vector.size() - 1;
		m_KeyToIndex[key] = index;
	}

	void eraseWithKey(const Key& key)
	{
		auto iterator = m_KeyToIndex.find(key);

		if (iterator != m_KeyToIndex.end())
		{
			size_t index = iterator->second;
			m_KeyToIndex.erase(iterator);
			m_Vector.erase(m_Vector.begin() + index);

			for (auto& pair : m_KeyToIndex)
			{
				if (pair.second > index)
				{
					pair.second--;
				}
			}
		}
		else
		{
			throw std::out_of_range("Key not found in KeyedVector eraseWithKey");
		}
	}

	void eraseWithIndex(size_t index)
	{
		if (index >= m_Vector.size()) { throw std::out_of_range("Index out of range in KeyedVector"); }

		bool foundKey = false;
		for (auto& pair : m_KeyToIndex)
		{
			if (pair.second == index)
			{
				m_KeyToIndex.erase(pair.first);
				foundKey = true;
				break;
			}
		}

		if (!foundKey) { throw std::out_of_range("Key not found in KeyedVector eraseWithIndex"); }

		m_Vector.erase(m_Vector.begin() + index);

		for (auto& pair : m_KeyToIndex)
		{
			if (pair.second > index)
			{
				pair.second--;
			}
		}
	}

private:
	std::vector<T> m_Vector;
	std::map<Key, size_t> m_KeyToIndex;
};
