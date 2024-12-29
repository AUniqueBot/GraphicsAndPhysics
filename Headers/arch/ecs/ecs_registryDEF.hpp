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



// ======================================================================================

template <typename T>
SparseSet<EntityID, T>& ComponentPool<T>::Data() {
	return m_compPool;
}

template <typename T>
const SparseSet<EntityID, T>& ComponentPool<T>::Data() const {
	return m_compPool;
}



// ======================================================================================



