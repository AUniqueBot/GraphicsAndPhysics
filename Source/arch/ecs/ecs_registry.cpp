#include "pch.h"
#include <arch/ecs/ecs_registry.h>


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
