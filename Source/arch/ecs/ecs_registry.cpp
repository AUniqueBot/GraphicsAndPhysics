#include "pch.h"
#include <arch/ecs/ecs_registry.h>


EntityRegistry::~EntityRegistry() {
	Clear();
}

void EntityRegistry::PrintDebugInfo() const {
	// to print - entities, registries
	std::stringstream ss;
	ss << typeid(EntityRegistry).name() << "::Debug Info\n";
	ss << "=============================================================\n\n";
	ss << "  entity count:    " << m_entityList.size() << "\n";
	ss << "  component count: " << m_componentPool.size() << "\n";
	for (auto& [compType, pool] : m_componentPool) {
		ss << "    ----------------\n";
		ss << "    component type: " << compType.name() << "\n";
		ss << "    pool size:      " << pool->size() << "\n";
		ss << "    ----------------\n";
	}

	ss << "\n=============================================================\n";
	std::cout << ss.str() ;
}


std::optional<std::reference_wrapper<Entity>> EntityRegistry::Instantiate() {
	Entity newEntt{this};
	EntityID refID = newEntt.GetID();
	m_entityList.Add(std::move(newEntt), newEntt.GetID());

	return m_entityList.At(refID);
}



void EntityRegistry::Clear() {
	// ensure no dangling references.
	m_compRegisterFunctions.clear();
}

