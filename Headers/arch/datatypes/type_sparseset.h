/*
	@brief contains the sparsemap type - a data type that allows you to lookup <T> based on <IDType>.
*/

#pragma once

#include <pch.h>
#include <unordered_map>


template <typename IDType, typename T>
class SparseSet {


public:
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

	std::vector<T>& Data()				{ return m_typeContainer; } ///<! try not to use this
	
	const std::vector<T>& Data() const	{ return m_typeContainer; } ///<! use this instead.


	// getters

	///! @brief accesses the data type given the [_entityID]
	///! @param _entityID: the id of the item associated with this.
	///! @return a std::optional of the data type. returns nullopt if the item doesn't exist
	std::optional<T&> operator[](IDType _entityID);
	
	std::optional<T&> At(IDType _entityID);
	std::optional<const T&> At(IDType _entityID) const;


private:
	std::vector<T> m_typeContainer;
	

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