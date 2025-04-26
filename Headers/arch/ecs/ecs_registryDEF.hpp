#pragma once

template <typename T>
std::optional<std::reference_wrapper<ComponentPool<T>>> EntityRegistry::GetComponentPool() {
	auto itr = m_componentPool.find(typeid(T));
	if (itr != m_componentPool.end())
		return std::ref(*std::static_pointer_cast<ComponentPool<T>>(itr->second));

	std::cout << "Non registered type called: " << typeid(T).name() << std::endl;
	return std::nullopt;
}


template <typename T>
std::optional<std::reference_wrapper<const ComponentPool<T>>> EntityRegistry::GetComponentPool() const {
	auto itr = m_componentPool.find(typeid(T));
	if (itr != m_componentPool.end())
		return std::cref(*std::static_pointer_cast<ComponentPool<T>>(itr->second));

	std::cout << "Non registered type called: " << typeid(T).name() << std::endl;
	return std::nullopt;
}




template <typename T>
bool EntityRegistry::AddComponent(EntityID _addTo) {
	auto val = GetComponentPool<T>();
	if (!val.has_value()) {
		std::cout << typeid(EntityRegistry).name() << ": Component type: \"" << typeid(T).name() << "\" undefined.\n";
		return false;
	}
	ComponentPool<T>& compPool = val.value().get();


	return compPool.Add(_addTo);
}
template <typename T>
bool EntityRegistry::RemoveComponent(EntityID _removeFrom) {
	auto val = GetComponentPool<T>();
	if (!val.has_value()) {
		std::cout << typeid(EntityRegistry).name() << ": Component type: \"" << typeid(T) << "\" undefined.\n";
		return false;
	}
	ComponentPool<T>& compPool = val.value().get();
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
bool ComponentPool<T>::Add(EntityID _addTo) {
	std::cout << "[[ ================================================================== ]]" << std::endl;
	std::cout << "Adding Component: " << typeid(T).name() << std::endl;
	std::cout << "Client: " << _addTo << std::endl;
	bool result = m_compPool.Add(T{}, _addTo);
	std::cout << (result ? "Good" : "Bad") << std::endl;
	std::cout << "Comp Pool Size: " << m_compPool.size() << std::endl;
	std::cout << "[[ ================================================================== ]]" << std::endl;
	return result;
}

template<typename T>
bool ComponentPool<T>::Remove(EntityID _removeFrom) {
	return m_compPool.Remove(_removeFrom);
}


// ======================================================================================

