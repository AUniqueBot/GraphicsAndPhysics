#pragma once
#include <pch.h>


template <typename K, typename V>
class BidirectionalMap {



public:

	V& operator[](const K& _key) {
		return m_forwardDir[_key];
	}
	K& operator[](const V& _val) {
		return m_backwardDir[_val];
	}

private:
	std::unordered_map<K, V> m_forwardDir;
	std::unordered_map<V, K> m_backwardDir;
};