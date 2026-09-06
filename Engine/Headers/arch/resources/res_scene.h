#pragma once
#include <pch.h>
#include <unordered_set>
#include <arch/common/entity.h>
#include <arch/common/entityid.h>
#include <arch/resources/res_resource.h>
#include <arch/datatypes/type_sparseSet.h>
#include <serialization/serialize_jsonfile.h>



// only responsible for holding a bunch of references to a scene.

class EntityRegistry;

struct EntityNode {
	EntityID m_entityId { EntityConstants::C_ENTITYID_INVALID };
	EntityID m_parentId { EntityConstants::C_ENTITYID_INVALID };
	std::unordered_set<EntityID> m_children;
};

class Scene : public Resource<Scene> {
public:

	bool AddEntity(EntityID _id);
	EntityView Instantiate(RES_ID _prefab = EntityConstants::C_ENTITYID_INVALID);
	bool Destroy(EntityView _entity, bool _recursive = true);
	bool Destroy(EntityID _id, bool _recursive = true);

public:
	// set to 0 to unparent.
	void Parent(EntityID _child, EntityID _parent = EntityConstants::C_ENTITYID_INVALID);
	bool DescendantOf(EntityID _toCheck, EntityID _parent) const;

public:
	EntityRegistry* Registry();
	const EntityRegistry* Registry() const;
	void Registry(EntityRegistry* _registry);

public:
	void LoadScene(const Serialization::JSONFile& _jsonData);
	static Scene LoadScene(const Serialization::JSONFile& _jsonData, EntityRegistry& _registry);
private:

	void ClearEntities();
private:
	EntityRegistry* m_registry;
	SparseSet<EntityID, EntityNode> m_sceneEntities;
};