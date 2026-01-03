#include "pch.h"
#include <arch/ecs/ecs_fwdDecl_entityRegistry.h>
#include <arch/components/comp_transform.h>
#include <arch/common/sparseSetViewHandles.h>

EntityRegistry::~EntityRegistry() {
	Clear();
}

void EntityRegistry::PrintDebugInfo() const {
	// to print - entities, registries
	std::stringstream ss;
	ss << typeid(EntityRegistry).name() << "::Debug Info\n";
	ss << "=============================================================\n\n";
	ss << "  entity count:    " << m_entityList.size() << "\n";
	ss << "  component count: " << m_componentMetadataMap.size() << "\n";
	for (auto& [compType, info] : m_componentMetadataMap) {
		const auto& pool = info.GetComponentPool();
		
		ss << "    ----------------\n";
		ss << "    component type: " << compType.name() << "\n";
		ss << "    pool size:      " << pool->size() << "\n";
		ss << "    ----------------\n";
	}

	ss << "\n=============================================================\n";
	std::cout << ss.str();
}


EntityView EntityRegistry::Instantiate() {
	Entity newEntt	{ this };
	EntityID refID = newEntt.GetID();
	m_entityList.Add(std::move(newEntt), refID);
	// map version.
	// m_entityList.Add(std::move(newEntt), newEntt.GetID());	
	// m_entityList.emplace(std::make_pair(refID, std::move(newEntt)));
	// return m_entityList.at(refID);

	// note to add a transform component.
	auto toRet = m_entityList.At(refID);
	toRet->AddComponent<Transform>(); // it will ALWAYS add a transform component.

	return m_entityList.At(refID);
}



void EntityRegistry::Clear() {
	// ensure no dangling references.
	m_componentMetadataMap.clear();
}

