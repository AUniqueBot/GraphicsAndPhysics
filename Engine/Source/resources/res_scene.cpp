#include <algorithm>
#include <arch/resources/res_scene.h>
#include <arch/ecs/ecs_registry.h>
#include <arch/core.h>


bool Scene::AddEntity(EntityID _id) {
	if (_id == EntityConstants::C_ENTITYID_INVALID) return false;

	EnsureUnique();
	return true;
}

EntityView Scene::Instantiate() {
	if (!m_registry) {
		return EntityView(std::nullopt);
	}
	EntityView entity = m_registry->Instantiate();
	if (!entity) {
		return entity;
	}
	// generate an entity node to push into the thing
	EntityNode entitynode;
	entitynode.m_entityId = entity->GetID();
	m_sceneEntities.Add(std::move(entitynode), entitynode.m_entityId);
	return entity;
}

bool Scene::RemoveEntity(EntityView _entity, bool _recursive) {
	if (!_entity) return false;
	return RemoveEntity(_entity->GetID(), _recursive);
}

bool Scene::RemoveEntity(EntityID _id, bool _recursive) {
	// dfs removal
	auto entitynode = m_sceneEntities.At(_id);
	if (!entitynode) return false;
	for (EntityID child : entitynode->m_children) {
		RemoveEntity(child, true);
	}
	return m_sceneEntities.Remove(_id);;
}

void Scene::Parent(EntityID _child, EntityID _parent) {
	if (_child == EntityConstants::C_ENTITYID_INVALID) {
		LOG_ERROR("What are you trying to do?");
		return;
	}
	if (_child == _parent) {
		LOG_WARN("parent cannot be the same as child.");
		return;
	}
	if (
		_child != EntityConstants::C_ENTITYID_INVALID && 
		_parent != EntityConstants::C_ENTITYID_INVALID && 
		DescendantOf(_parent, _child)
		) {
		LOG_WARN("Operation would cause dependency loop. Aborting. child: [" 
			<< _child << "], parent: [" << _parent<< "].");
		return;
	}

	// find child and to be parent.
	auto child = m_sceneEntities.At(_child);
	if (!child) {
		LOG_WARN("Cannot find entity: [" << _child << "]");
		return;
	}
	auto newParent = m_sceneEntities.At(_parent);
	if (_parent != EntityConstants::C_ENTITYID_INVALID && !newParent) {
		LOG_WARN("Cannot find entity: [" << _parent << "]");
		return;
	}


	// deparent old. remove child from old
	auto oldParent = m_sceneEntities.At(child->m_parentId);
	if (oldParent) {
		oldParent->m_children.erase(_child);
	}

	if (newParent) {
		newParent->m_children.insert(_child);
	}

	child->m_parentId = _parent;

}

bool Scene::DescendantOf(EntityID _toCheck, EntityID _parent) const {
	if (_parent == _toCheck) {
		LOG_WARN("Parent is same as checked.");
		return true;
	}

	auto parentView = m_sceneEntities.At(_parent);
	// extra conditional
	if (!parentView) return false;

	if (parentView->m_children.contains(_toCheck)) return true;
	for (EntityID child: parentView->m_children) {
		if (DescendantOf(child, _toCheck)) {
			return true;
		}
	}
	return false;
}

EntityRegistry* Scene::Registry() {
	return m_registry;
}

const EntityRegistry* Scene::Registry() const {
	return m_registry;
}

void Scene::Registry(EntityRegistry* _registry) {
	m_registry = _registry;
}

void Scene::EnsureUnique() {

}

void Scene::ClearEntities() {
	for (EntityNode entity : m_sceneEntities) {

	}
}
