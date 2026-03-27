#include "entity.h"
#pragma once

template <typename T>
void Entity::AddComponent() {
	if (m_registry == nullptr) return;
	bool res = m_registry->AddComponent<T>(m_id);
	std::stringstream ss;
	ss << "Adding Component <" << Reflection::TypeName<T>() << ">: " << (res ? "Pass" : "FAIL") << std::endl;
	if (res) {
		LOG_INFO(ss.str());
	}
	else {
		LOG_ERROR(ss.str());
	}


	auto compPool = m_registry->GetComponentPool<T>();

	if (compPool) {
		ss.clear();
		ss << "current count: " << compPool->size() << std::endl;

		LOG_INFO(ss.str());

		compPool->Get(m_id)->Init();
	}

}


template <typename T>
void Entity::RemoveComponent() {
	if (m_registry == nullptr) return;
	bool res = m_registry->RemoveComponent<T>(m_id);

	std::stringstream ss;
	ss << "Removing Component <" << Reflection::TypeName<T>() << ">: " << (res ? "Pass" : "FAIL") << std::endl;
	if (res) {
		LOG_INFO(ss.str());
	}
	else {
		LOG_ERROR(ss.str());
	}

	auto compPool = m_registry->GetComponentPool<T>();

	if (compPool != std::nullopt) {
		ss.clear();
		ss << "current count: " << compPool.value().get().size() << std::endl;
		
		LOG_INFO(ss.str());
		
		compPool.get().Get(m_id)->End();
	}

}

template<typename T>
ComponentView<T> Entity::GetComponent() const {
	if (!m_registry) 
		return ComponentView<T>(std::nullopt);
	SparseSetView<ComponentPool<T>> compPoolValue
		= m_registry->GetComponentPool<T>();
	
	if (!compPoolValue) return ComponentView<T>(std::nullopt);
	
	ComponentPool<T>& compPool = *compPoolValue;
	return compPool.Get(m_id);
}


