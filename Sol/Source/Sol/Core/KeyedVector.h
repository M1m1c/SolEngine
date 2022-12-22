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

    // Element access
    T& operator[](const Key& key) {
        // Check if the key is already in the map
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

    const T& operator[](const Key& key) const {
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

private:
    std::vector<T> m_Vector;
    std::map<Key, size_t> m_KeyToIndex;
};
