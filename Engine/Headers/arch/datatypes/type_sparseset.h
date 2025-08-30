/*
	@brief contains the sparsemap type - a data type that allows you to lookup <T> based on <IDType>.
*/
#pragma once

#include <pch.h>
#include <arch/common/sparseSetViewHandles.h>


#ifndef TYPE_SPARSESET
#define TYPE_SPARSESET




template <typename IDType, typename T>
class SparseSet {


public:
	SparseSet() = default;
	~SparseSet() { clear(); };


	///! @brief adds [_newItem] paired to a specific ID
	///! @param _newItem: the data paired to the id
	///! @param _id: identifier
	///! @return true if adding is successful. false for anything else.
	bool Add(T&& _newItem, IDType _id);


	///! @brief removes the data type associated with [_id]
	///! @param _id: the id that you want the data to remove.
	///! @return returns true if removal is successful. false for anything else.
	bool Remove(IDType _id);

	// getters for the container

	std::deque<T>& Data()				{ return m_typeContainer; } ///<! try not to use this
	
	const std::deque<T>& Data() const	{ return m_typeContainer; } ///<! use this instead.


public:



	///! @brief accesses the data type given the [_entityID]
	///! @param _entityID: the id of the item associated with this.
	///! @return a std::optional of the data type. returns nullopt if the item doesn't exist
	SparseSetView<T> operator[](IDType _entityID);
	
	/// @brief accesses the data type given the [_entityID]
	///! @param _entityID: the id of the item associated with this.
	///! @return a std::optional of the data type. returns nullopt if the item doesn't exist
	SparseSetView<T> At(IDType _entityID);
	SparseSetView<const T> At(IDType _entityID) const;


	// functions to enable for range loop functionality
	
	///! @brief provides the iterator to the beginning of the sparse set.
	///! @return iterator to the beginning of the sparse set. (stored as a vector)
	auto begin()							{ return m_typeContainer.begin(); }
	auto begin() const				{ return m_typeContainer.begin(); }

	///! @brief provides the iterator to the end of the sparse set.
	///! @return iterator to the end of the iterator. (stored as a vector)
	auto end()							{ return m_typeContainer.end(); }
	auto end() const					{ return m_typeContainer.end(); }


	size_t size() const					{ return m_typeContainer.size(); }


	void clear()						{ m_typeContainer.clear(); m_idxToValue.clear(); m_valueToIdx.clear(); }


private:
	// the container containing the type's data.
	std::deque<T> m_typeContainer;
	

private:
	// mapping data structs

	//! @brief this container maps ids to an entity.
	// KEY = Index, VALUE = EntityID 
	std::vector<IDType> m_idxToValue;

	//! @brief this container maps entities to ids.
	//! KEY = EntityID, VALUE = Index
	std::unordered_map<IDType, int> m_valueToIdx;


};
#include <arch/datatypes/type_sparsesetDEF.hpp>

#endif