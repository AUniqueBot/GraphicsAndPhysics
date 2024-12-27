/*
	@brief contains the sparsemap type - a data type that allows you to lookup <T> based on <IDType>.
*/

#pragma once

#include <pch.h>
#include <unordered_map>

#include <arch/common/entity.h>


template <typename IDType, typename T>
class SparseMap {


public:

	bool Add(T&& _newItem, IDType _id);
	bool Remove(IDType _id);

	// getters for the container


	std::vector<T>& GetContainer()				{ return m_typeContainer; } //! try not to use this
	
	const std::vector<T>& GetContainer() const	{ return m_typeContainer; }


	// getter

	//! @brief gets the data based on the id provided.
	std::optional<T&> operator[](IDType _entityID);
	
	std::optional<T&> At(IDType _entityID);
	std::optional<const T&> At(IDType _entityID) const;


private:
	std::vector<T> m_typeContainer;
	

private:
	// mapping data structs

	//! @brief this container maps ids to an entity.
	// KEY = Index, VALUE = EntityID 
	std::vector<EntityID> m_idxToEntity;

	//! @brief this container maps entities to ids.
	//! KEY = EntityID, VALUE = Index
	std::unordered_map<EntityID, int> m_entityToIdx;


};


#include <arch/ecs/ecs_sparsemapDEF.hpp>