#include "pch.h"
#include <arch/ecs/ecs_fwdDecl_entityRegistry.h>
#include <arch/components/comp_transform.h>
#include <arch/datatypes/type_sparseSet_viewHandle.h>




EntityRegistry::~EntityRegistry() {
	Clear();
}

void EntityRegistry::PrintDebugInfo() const {
	// to print - entities, registries
	std::stringstream ss;
	ss << typeid(EntityRegistry).name() << "::Debug Info\n";
	ss << "=============================================================\n\n";
	ss << "  entity count:    " << m_entityList.size() << "\n";
	ss << "  component count: " << m_componentData.size() << "\n";
	for (auto& [compType, packedData] : m_componentData) {
		const auto& pool = packedData.m_componentPool;
		
		ss << "    ----------------\n";
		ss << "    component type: " << compType.name() << "\n";
		ss << "    pool size:      " << pool->size() << "\n";
		ss << "    ----------------\n";
	}

	ss << "\n=============================================================\n";
	std::cout << ss.str();
}

std::vector<ComponentHandle> EntityRegistry::GetEntityComponents(const EntityID& _entityId) {
	std::vector<ComponentHandle> comps;
	EntityView entity{ GetEntity(_entityId) };

	for (const CompID& compId : entity->GetAttachedComponents()) {
		// access.
		SparseSetView<CompTypeID> compType{ m_componentIDLookup.At(compId) };
		if (!compType) continue;
		Component* ptr = m_componentData.at(*compType).m_componentPool->GetComponent(_entityId);
		if (ptr) comps.push_back(ComponentHandle{ ptr, &m_componentData.at(*compType).m_componentMetadata });
	}
	return comps;
}

const std::vector<ComponentHandle>& EntityRegistry::GetEntityComponents(const EntityID& _entityId) const {
	std::vector<ComponentHandle> comps;
	EntityViewConst entity { GetEntity(_entityId) };

	for (const CompID& compId : entity->GetAttachedComponents()) {
		// access.
		SparseSetView<const CompTypeID> compType{ m_componentIDLookup.At(compId) };
		if (!compType) continue;
		Component* ptr = m_componentData.at(*compType).m_componentPool->GetComponent(_entityId);
		if (ptr) comps.push_back(ComponentHandle{ptr});
	}
	// TODO: insert return statement here
	return comps;
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

void EntityRegistry::Destroy(Entity _remove, bool _recursiveDeleteChildren) {
	// do child recursion here.
	(void)_recursiveDeleteChildren;


	for (unsigned _id : _remove.GetAttachedComponents()) {
		SparseSetView compTypeIDView = m_componentIDLookup.At(_id);
		if (!compTypeIDView) continue;
		const EntityRegistry::CompTypeID& compTypeID = *compTypeIDView;
		m_componentData.at(compTypeID).m_componentPool->Remove(_remove.GetID());
	}
	m_entityList.Remove(_remove.GetID());
}

void EntityRegistry::Destroy(EntityID _id, bool _recursiveDeleteChildren) {
	if (EntityID::ENTITYID_INVALID == _id) return;
	EntityView e { GetEntity(_id) };
	if (e) Destroy(*e, _recursiveDeleteChildren);
}



void EntityRegistry::SelectEntity(EntityID _selectedObject, bool _additive) {
	if (!_selectedObject.IsValid() && !_additive) {
		ClearSelection();
		return;
	}


	if (!_additive) {
		m_selectedEntitiesList.clear();
		m_selectedEntitiesList.push_back(_selectedObject);
		return;
	}

	// search if object is inside.
	auto itr = std::find(m_selectedEntitiesList.begin(), m_selectedEntitiesList.end(), _selectedObject);
	if (itr != m_selectedEntitiesList.end()) {
		// rotate to end.
		std::rotate(itr, std::next(itr), m_selectedEntitiesList.end());
	}
	else {
		m_selectedEntitiesList.push_back(_selectedObject);
	}
}



const std::vector<EntityID>& EntityRegistry::SelectedEntities() const {
	return m_selectedEntitiesList;
}

EntityID EntityRegistry::SelectedEntity() const {
	return m_selectedEntitiesList.size() > 0 ? m_selectedEntitiesList.back() : EntityID::ENTITYID_INVALID;
}


void EntityRegistry::DeselectEntity(EntityID _selectedID) {
	if (EntityID::ENTITYID_INVALID == _selectedID) return;
	
	// case of deselecting the most current selection
	if (SelectedEntity() == _selectedID) {
		m_selectedEntitiesList.pop_back();
		return;
	}

	// deselecting something that is somewhere else on the list
	auto itr = std::find(m_selectedEntitiesList.begin(), m_selectedEntitiesList.end(), _selectedID);
	// swap pop idiom
	std::rotate(itr, itr + 1, m_selectedEntitiesList.end());
	m_selectedEntitiesList.pop_back();
}



void EntityRegistry::ClearSelection() {
	m_selectedEntitiesList.clear();
}

bool EntityRegistry::EntityIsSelected(EntityID _id, bool _isCurrentSelection) const {
	if (!m_selectedEntitiesList.size()) {
		return false;
	}
	
	if (_isCurrentSelection) {
		return m_selectedEntitiesList.back() == _id;
	}

	auto itr = std::find(m_selectedEntitiesList.begin(), m_selectedEntitiesList.end(), _id);
	return itr != m_selectedEntitiesList.end();
}


void EntityRegistry::Clear() {
	// ensure no dangling references.
	m_componentData.clear();
}

