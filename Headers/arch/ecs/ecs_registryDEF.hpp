#pragma once
#include <arch/ecs/ecs_registry.h>



template <typename T>
void EntityRegistry::RegisterType() {
	// create a sparse set of type T
	m_componentPool[typeid(T)] = std::make_shared<ComponentPool<T>>();
}

template <typename T>
std::optional<ComponentPool<T>&> EntityRegistry::GetComponentPool() {
	auto itr = m_componentPool.find(typeid(T));
	if (itr != m_componentPool.end()) return *std::static_pointer_cast<ComponentPool<T>>(itr->second);

	// error log here.
	std::cerr << "Non registered type called: " << typeid(T).name() << std::endl;

	return std::nullopt;
}


template <typename T>
const std::optional<ComponentPool<T>&> EntityRegistry::GetComponentPool() const {
	auto itr = m_componentPool.find(typeid(T));
	if (itr != m_componentPool.end()) return *std::static_pointer_cast<ComponentPool<T>>(itr->second);

	// error log here.
	std::cerr << "Non registered type called: " << typeid(T).name() << std::endl;

	return std::nullopt;
}




template <typename T>
bool EntityRegistry::AddComponent(EntityID _addTo) {
	auto val = GetComponentPool<T>();
	if (!val.has_value()) {
		std::cerr << typeid(EntityRegistry).name() << ": Component type: \"" << typeid(T) << "\" undefined.";
		return false;
	}
	ComponentPool<T>& compPool = val.value();
	return compPool.Add(_addTo);
}
template <typename T>
bool EntityRegistry::RemoveComponent(EntityID _removeFrom) {
	auto val = GetComponentPool<T>();
	if (!val.has_value()) {
		std::cerr << typeid(EntityRegistry).name() << ": Component type: \"" << typeid(T) << "\" undefined.";
		return false;
	}
	ComponentPool<T>& compPool = val.value();
	return compPool.Remove(_removeFrom);
}


// ======================================================================================

template <typename T>
SparseSet<EntityID, T>& ComponentPool<T>::Data() {
	return m_compPool;
}

template <typename T>
const SparseSet<EntityID, T>& ComponentPool<T>::Data() const {
	return m_compPool;
}

template<typename T>
inline bool ComponentPool<T>::Add(EntityID _addTo) {
	return m_compPool.Add(_addTo);
}

template<typename T>
inline bool ComponentPool<T>::Remove(EntityID _removeFrom) {
	return m_compPool.Remove(_removeFrom);
}



// ======================================================================================



