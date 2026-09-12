#pragma once
#include <arch/datatypes/type_sparseset.h>


template <typename KeyType, typename ValueType>
bool SparseSet<KeyType, ValueType>::add(ValueType&& _newItem, KeyType _id) {

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

template <typename KeyType, typename ValueType>
bool SparseSet<KeyType, ValueType>::add(const ValueType& _newItem, KeyType _id) {

	// checks if there's a value for that entity existing already
	if (m_valueToIdx.contains(_id)) {
		LOG_WARN("Attempting to add an item for a registered client.");
		return false;
	}
	// add this value 
	m_valueToIdx[_id] = static_cast<int>(m_idxToValue.size());
	m_idxToValue.push_back(_id);
	m_typeContainer.push_back(_newItem);
	return true;
}


template <typename KeyType, typename ValueType>
bool SparseSet<KeyType, ValueType>::remove(KeyType _id){
	
	// find if this key exists
	if (!m_valueToIdx.contains(_id)) return false;


	// remove all instances of that entity from the maps and remove the entity.
	
	// first get the index of the removed element and the back element.
	int toRemoveIndex				= m_valueToIdx.at(_id);
	KeyType backIdx					= m_idxToValue.back();

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

template <typename KeyType, typename ValueType>
SparseSetView<ValueType> SparseSet<KeyType, ValueType>::operator[](KeyType _entityID) {
	if (m_valueToIdx.contains(_entityID)) {
		return SparseSetView<ValueType>(m_typeContainer[m_valueToIdx[_entityID]]);
	}
	return SparseSetView<ValueType>(std::nullopt);
}

template <typename KeyType, typename ValueType>
inline SparseSetView<ValueType> SparseSet<KeyType, ValueType>::at(KeyType _entityID) {
	if (m_valueToIdx.contains(_entityID)) {
		return SparseSetView<ValueType>(
			std::ref(
				m_typeContainer.at(m_valueToIdx.at(_entityID))
			));
	}
	return SparseSetView<ValueType>(std::nullopt);
}

template <typename KeyType, typename ValueType>
inline SparseSetView<const ValueType> SparseSet<KeyType, ValueType>::at(KeyType _entityID) const {
	if (m_valueToIdx.contains(_entityID)) {
		return SparseSetView<const ValueType>(
			std::ref(
				m_typeContainer.at(m_valueToIdx.at(_entityID))
			));
	}
	return SparseSetView<const ValueType>(std::nullopt);
}
