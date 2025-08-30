#include "ecs_registry.h"
#include <arch/common/sparseSetViewHandles.h>
#pragma once

template <typename T>
SparseSetView<ComponentPool<T>>  EntityRegistry::GetComponentPool() {
	auto itr = m_componentPool.find(typeid(T));
	if (itr != m_componentPool.end())
		return SparseSetView<ComponentPool<T>>{
			std::ref(*std::static_pointer_cast<ComponentPool<T>>(itr->second)) 
		};


	LOG_WARN("Non Registered Typed Called. Returning nullopt");
	return SparseSetView<ComponentPool<T>>(std::nullopt);
}


template <typename T>
SparseSetView<const ComponentPool<T>> EntityRegistry::GetComponentPool() const {
	auto itr = m_componentPool.find(typeid(T));
	if (itr != m_componentPool.end())
		return std::cref(*std::static_pointer_cast<ComponentPool<T>>(itr->second));

	LOG_WARN("Non Registered Typed Called. Returning nullopt");
	return SparseSetView<const ComponentPool<T>>(std::nullopt);
}




template <typename T>
bool EntityRegistry::AddComponent(EntityID _addTo) {
	auto val = GetComponentPool<T>();
	if (!val) {
		std::stringstream ss;
		ss << "Component type: \"" << typeid(T).name() << "\" undefined.\n";
		LOG_WARN(ss.str());
		return false;
	}

	return val->Add(_addTo);
}
template <typename T>
bool EntityRegistry::RemoveComponent(EntityID _removeFrom) {
	auto val = GetComponentPool<T>();
	if (!val.has_value()) {
		std::stringstream ss;
		ss << "Component type: \"" << typeid(T).name() << "\" undefined.\n";
		LOG_WARN(ss.str());
		return false;
	}
	ComponentPool<T>& compPool = val.value().get();
	return compPool.Remove(_removeFrom);
}

template<typename T>
inline bool EntityRegistry::ComponentPoolExists() {
	return static_cast<bool>(GetComponentPool<T>());
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
bool ComponentPool<T>::Add(EntityID _addTo) {
	std::cout << "[[ ================================================================== ]]" << std::endl;
	std::cout << "Adding Component: " << typeid(T).name() << std::endl;
	std::cout << "Client: " << _addTo << std::endl;
	bool result = m_compPool.Add(T{}, _addTo);
	
	std::cout << (result ? "Good" : "Bad") << std::endl;
	std::cout << "Comp Pool Size: " << m_compPool.size() << std::endl;
	std::cout << "[[ ================================================================== ]]" << std::endl;
	
	if (result) {
		ComponentView<T> component = m_compPool.At(_addTo);
		component->SetEntityID(_addTo);
	}
	
	return result;
}

template<typename T>
bool ComponentPool<T>::Remove(EntityID _removeFrom) {
	return m_compPool.Remove(_removeFrom);
}

template<typename T>
inline ComponentView<T> ComponentPool<T>::Get(EntityID _client) {
	return m_compPool.At(_client);
}


template<typename T>
inline ComponentView<const T> ComponentPool<T>::Get(EntityID _client) const {
	return m_compPool.At(_client);
}



// ======================================================================================

