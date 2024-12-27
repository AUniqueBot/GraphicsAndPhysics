#pragma once
#include <arch/ecs/ecs_sparsemap.h>


template <typename IDType, typename T>
bool SparseMap<IDType, T>::Add(T&& _newItem, IDType _id) {

	// checks if there's a value for that entity existing already
	if (m_entityToIdx.contains(_id)) return false;

	// add this value 
	m_entityToIdx[_id] = m_idxToEntity.size();
	m_idxToEntity.push_back(_id);

	m_typeContainer.push_back(std::move(_newItem));


	return true;
}

template <typename IDType, typename T>
bool SparseMap<IDType, T>::Remove(IDType _id){
	
	// find if this key exists
	if (!m_entityToIdx.contains(_id)) return false;


	// remove all instances of that entity from the maps and remove the entity.
	
	// first get the index of the removed element and the back element.
	int toRemoveIndex				= m_entityToIdx.at(_id);
	EntityID backIdx				= m_idxToEntity.back();

	// swap the elements in the container and the idxToEntity

	m_typeContainer[toRemoveIndex]	= std::move(m_typeContainer.back());
	m_idxToEntity[toRemoveIndex]	= std::move(m_idxToEntity.back());

	// replace the value of the back
	m_entityToIdx[backIdx]			= toRemoveIndex;
	
	// remove excess elements
	m_entityToIdx.erase(_id);
	m_idxToEntity.pop_back();
	m_typeContainer.pop_back();


	return true;
}

template <typename IDType, typename T>
std::optional<T&> SparseMap<IDType, T>::operator[](IDType _entityID) {
	if (m_entityToIdx.contains(_entityID)) {
		return m_typeContainer[m_entityToIdx[_entityID]];
	}
	return std::nullopt;
}

template <typename IDType, typename T>
inline std::optional<T&> SparseMap<IDType, T>::At(IDType _entityID) {
	if (m_entityToIdx.contains(_entityID)) {
		return m_typeContainer[m_entityToIdx[_entityID]];
	}
	return std::nullopt;
}

template <typename IDType, typename T>
inline std::optional<const T&> SparseMap<IDType, T>::At(IDType _entityID) const {
	if (m_entityToIdx.contains(_entityID)) {
		return m_typeContainer[m_entityToIdx[_entityID]];
	}
	return std::nullopt;
}
