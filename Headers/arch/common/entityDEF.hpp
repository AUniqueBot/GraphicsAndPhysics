#include "entity.h"
#pragma once

template <typename T>
void Entity::AddComponent() {
	if (m_registry == nullptr) return;
	bool res = m_registry->AddComponent<T>(m_id);
	std::cout << "Adding Component <"<< typeid(T).name() << ">: " << (res? "Pass" : "FAIL") << std::endl;


	auto compPool = m_registry->GetComponentPool<T>();

	if (compPool != std::nullopt) {
		std::cout << typeid(compPool.value().get()).name() << "current count: " << compPool.value().get().size() << std::endl;
	}



}


template <typename T>
void Entity::RemoveComponent() {
	if (m_registry == nullptr) return;
	bool res = m_registry->RemoveComponent<T>(m_id);
	std::cout << "Removing Component <" << typeid(T).name() << ">: " << (res ? "Pass" : "FAIL") << std::endl;


	auto compPool = m_registry->GetComponentPool<T>();

	if (compPool != std::nullopt) {
		std::cout << typeid(compPool.value().get()).name() << "current count: " << compPool.value().get().size() << std::endl;
	}

}

template<typename T>
std::optional<std::reference_wrapper<T>> Entity::GetComponent() {
	if (m_registry == nullptr) return std::nullopt;
	std::optional<std::reference_wrapper<ComponentPool<T>>> compPoolValue
		= m_registry->GetComponentPool<T>();
	if (!compPoolValue.has_value()) return std::nullopt;
	ComponentPool<T>& compPool = compPoolValue.value().get();
	return compPool.Get(m_id);
}

template<typename T>
std::optional<std::reference_wrapper<const T>> Entity::GetComponent() const {
	if (m_registry == nullptr) return std::nullopt;
	std::optional<std::reference_wrapper<const ComponentPool<T>>> compPoolValue 
		= m_registry->GetComponentPool<T>();
	if (!compPoolValue.has_value()) return std::nullopt;
	const ComponentPool<T>& compPool = compPoolValue.value().get();
	return compPool.Get(m_id);
}
