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