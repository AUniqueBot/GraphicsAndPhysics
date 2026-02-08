#pragma once
#include <arch/datatypes/type_sparseset.h>


template <typename IDType, typename T>
bool SparseSet<IDType, T>::Add(T&& _newItem, IDType _id) {

	// checks if there's a value for that entity existing already
	if (m_valueToIdx.contains(_id)) {
		LOG_WARN("Attempting to add an item for a registered client.");
		return false;
	}
	// add this value 
	m_valueToIdx[_id] = static_cast<int>(m_idxToValue.size());
	m_idxToValue.push_back(_id);

	m_typeContainer.push_back(std::move(_newItem));

	return true;
}

template <typename IDType, typename T>
bool SparseSet<IDType, T>::Remove(IDType _id){
	
	// find if this key exists
	if (!m_valueToIdx.contains(_id)) return false;


	// remove all instances of that entity from the maps and remove the entity.
	
	// first get the index of the removed element and the back element.
	int toRemoveIndex				= m_valueToIdx.at(_id);
	IDType backIdx					= m_idxToValue.back();

	// swap the elements in the container and the idxToEntity

	m_typeContainer[toRemoveIndex]	= std::move(m_typeContainer.back());
	m_idxToValue[toRemoveIndex]		= std::move(m_idxToValue.back());

	// replace the value of the back
	m_valueToIdx[backIdx]			= toRemoveIndex;
	
	// remove excess elements
	m_valueToIdx.erase(_id);
	m_idxToValue.pop_back();
	m_typeContainer.pop_back();


	return true;
}

template <typename IDType, typename T>
SparseSetView<T> SparseSet<IDType, T>::operator[](IDType _entityID) {
	if (m_valueToIdx.contains(_entityID)) {
		return SparseSetView<T>(m_typeContainer[m_valueToIdx[_entityID]]);
	}
	return SparseSetView<T>(std::nullopt);
}

template <typename IDType, typename T>
inline SparseSetView<T> SparseSet<IDType, T>::At(IDType _entityID) {
	if (m_valueToIdx.contains(_entityID)) {
		return SparseSetView<T>(
			std::ref(
				m_typeContainer.at(m_valueToIdx.at(_entityID))
			));
	}
	return SparseSetView<T>(std::nullopt);
}

template <typename IDType, typename T>
inline SparseSetView<const T> SparseSet<IDType, T>::At(IDType _entityID) const {
	if (m_valueToIdx.contains(_entityID)) {
		return SparseSetView<const T>(
			std::ref(
				m_typeContainer.at(m_valueToIdx.at(_entityID))
			));
	}
	return SparseSetView<const T>(std::nullopt);
}
