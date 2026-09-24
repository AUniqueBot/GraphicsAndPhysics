#include "ecs_registry.h"
#include <arch/datatypes/type_sparseSet_viewHandle.h>
#pragma once

template<std::derived_from<Component> T>
SparseSetView<ComponentPool<T>> EntityRegistry::GetComponentPool() {
	auto itr = m_componentData.find(typeid(T));
	if (itr != m_componentData.end()) {
		ComponentPackedData& componentInfo = itr->second;
		return SparseSetView<ComponentPool<T>>{
			std::ref(
				*std::static_pointer_cast<ComponentPool<T>>(componentInfo.m_componentPool)
			)
		};
	}

	LOG_WARN("Non Registered Typed Called. Returning nullopt");
	return SparseSetView<ComponentPool<T>>(std::nullopt);
}


template<std::derived_from<Component> T>
SparseSetView<const ComponentPool<T>> EntityRegistry::GetComponentPool() const {
	auto itr = m_componentData.find(typeid(T));
	if (itr != m_componentData.end()) {
		const ComponentPackedData& componentInfo = itr->second;
		return SparseSetView<const ComponentPool<T>>{
			std::cref(
				*std::static_pointer_cast<ComponentPool<T>>(componentInfo.m_componentPool)
			)
		};
	}

	LOG_WARN("Non Registered Typed Called. Returning nullopt");
	return SparseSetView<const ComponentPool<T>>(std::nullopt);
}




template<std::derived_from<Component> T>
bool EntityRegistry::AddComponent(EntityID _addTo) {
	return AddComponent(_addTo, typeid(T));
}


template<std::derived_from<Component> T>
bool EntityRegistry::RemoveComponent(EntityID _removeFrom) {
	return RemoveComponent(_removeFrom, typeid(T));
}

template<std::derived_from<Component> T>
inline bool EntityRegistry::ComponentPoolExists() const {
	return static_cast<bool>(GetComponentPool<T>());
}




// ======================================================================================

template<std::derived_from<Component> T>
SparseSet<EntityID, T>& ComponentPool<T>::Data() {
	return m_compPool;
}

template<std::derived_from<Component> T>
const SparseSet<EntityID, T>& ComponentPool<T>::Data() const {
	return m_compPool;
}

template<std::derived_from<Component> T>
bool ComponentPool<T>::Add(EntityID _addTo) {
	if (_addTo == EntityConstants::C_ENTITYID_INVALID) return false;
	bool result = m_compPool.add(T{}, _addTo);
	//std::cout << "[[ ================================================================== ]]" << std::endl;
	//std::cout << "Adding Component: " << typeid(T).name() << std::endl;
	//std::cout << "Client: " << _addTo << std::endl;
	
	//std::cout << (result ? "Good" : "Bad") << std::endl;
	//std::cout << "Comp Pool Size: " << m_compPool.size() << std::endl;
	//std::cout << "[[ ================================================================== ]]" << std::endl;
	
	if (result) {
		ComponentView<T> component = m_compPool[_addTo];
		component->SetEntityID(_addTo);
	}
	
	return result;
}

template<std::derived_from<Component> T>
bool ComponentPool<T>::Remove(EntityID _removeFrom) {
	if (_removeFrom == EntityConstants::C_ENTITYID_INVALID) return false;
	return m_compPool.remove(_removeFrom);
}

template<std::derived_from<Component> T>
inline ComponentView<T> ComponentPool<T>::Get(EntityID _client) {
	
	return m_compPool[_client];
}


template<std::derived_from<Component> T>
inline ComponentView<const T> ComponentPool<T>::Get(EntityID _client) const {
	return m_compPool.at(_client);
}





// ======================================================================================

