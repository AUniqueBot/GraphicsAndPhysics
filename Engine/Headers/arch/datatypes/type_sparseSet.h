/*
	@brief contains the sparsemap type - a data type that allows you to lookup <T> based on <IDType>.
*/
#pragma once
#ifndef TYPE_SPARSESET
#define TYPE_SPARSESET




#include <pch.h>
#include <arch/datatypes/type_sparseSet_viewHandle.h>


// why is it a deque?
template <typename KeyType, typename ValueType>
class SparseSet {


public:
	SparseSet() = default;
	~SparseSet() { clear(); };


	///! @brief adds [_newItem] paired to a specific ID
	///! @param _newItem: the data paired to the id
	///! @param _id: identifier
	///! @return true if adding is successful. false for anything else.
	bool add(ValueType&& _newItem, KeyType _id);
	bool add(const ValueType& _newItem, KeyType _id);


	///! @brief removes the data type associated with [_id]
	///! @param _id: the id that you want the data to remove.
	///! @return returns true if removal is successful. false for anything else.
	bool remove(KeyType _id);

	// getters for the container

	std::vector<ValueType>& data()				{ return m_typeContainer; } ///<! try not to use this
	
	const std::vector<ValueType>& data() const	{ return m_typeContainer; } ///<! use this instead.


public:



	///! @brief accesses the data type given the [_entityID]
	///! @param _entityID: the id of the item associated with this.
	///! @return a std::optional of the data type. returns nullopt if the item doesn't exist
	SparseSetView<ValueType> operator[](KeyType _entityID);
	
	/// @brief accesses the data type given the [_entityID]
	///! @param _entityID: the id of the item associated with this.
	///! @return a std::optional of the data type. returns nullopt if the item doesn't exist
	SparseSetView<ValueType> at(KeyType _entityID);
	SparseSetView<const ValueType> at(KeyType _entityID) const;


	// functions to enable for range loop functionality
	
	///! @brief provides the iterator to the beginning of the sparse set.
	///! @return iterator to the beginning of the sparse set. (stored as a vector)
	auto begin()								{ return m_typeContainer.begin(); }
	auto begin() const							{ return m_typeContainer.begin(); }

	///! @brief provides the iterator to the end of the sparse set.
	///! @return iterator to the end of the iterator. (stored as a vector)
	auto end()									{ return m_typeContainer.end(); }
	auto end() const							{ return m_typeContainer.end(); }


	size_t size() const							{ return m_typeContainer.size(); }


	void clear()								{ m_typeContainer.clear(); m_idxToValue.clear(); m_valueToIdx.clear(); }
public:

	const KeyType& getKey(int _idx) const		{ return m_idxToValue.at(_idx); }
	ValueType& getValue(int _idx)				{ return m_typeContainer.at(_idx); }
	const ValueType& getValue(int _idx)	const	{ return m_typeContainer.at(_idx); }

	bool contains(const KeyType& _key) const	{ return m_valueToIdx.contains(_key); };
	bool contains(const ValueType& _val) const {
		auto itr = std::find(m_typeContainer.begin(), m_typeContainer.end(), _val);
		return itr != m_typeContainer.end();
	};

	std::optional<KeyType> getKeyFromValue(const ValueType& _val) const {
		auto itr = std::find(m_typeContainer.begin(), m_typeContainer.end(), _val);
		if (itr == m_typeContainer.end()) return std::nullopt;
		size_t idx = itr - m_typeContainer.begin();
		return m_idxToValue.at(idx);
	}


private:
	// the container containing the type's data.
	std::vector<ValueType> m_typeContainer;
	

private:
	// mapping data structs

	//! @brief this container maps ids to an entity.
	// KEY = Index, VALUE = EntityID 
	std::vector<KeyType> m_idxToValue;

	//! @brief this container maps entities to ids.
	//! KEY = EntityID, VALUE = Index
	std::unordered_map<KeyType, int> m_valueToIdx;


};
#include <arch/datatypes/type_sparsesetDEF.hpp>

#endif