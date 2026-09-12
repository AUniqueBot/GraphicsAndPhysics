#pragma once
#include <pch.h>
#include <unordered_set>
#include <arch/common/entityid.h>

#include <arch/resources/res_resource.h>
#include <arch/resources/res_resourceHandle.h>

#include <arch/datatypes/type_sparseSet.h>
#include <serialization/serialize_jsonfile.h>



// only responsible for holding a bunch of references to a scene.

class EntityRegistry;
class AssetManager;

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
	EntityRegistry* GetRegistry();
	const EntityRegistry* GetRegistry() const;
	void SetRegistry(EntityRegistry* _registry);
public:

	AssetManager* GetAssetManager();
	const AssetManager* GetAssetManager() const;
	void SetAssetManager(AssetManager* _registry);

public:
	void SetSceneJSONData(
		const Serialization::JSONValue& _jsonData
	);

	
public:
	// - loading and saving -----------------------------
	void Load();
	static std::shared_ptr<Scene> LoadScene(
		Serialization::JSONFile& _jsonData, 
		EntityRegistry& _registry,
		AssetManager& _assetManager
		);

	void Save();
	Serialization::JSONValue Serialize(Serialization::JSONAllocator& _allocator, AssetManager& _asMgr) override;
	
	Serialization::JSONFile& GetJSONData();
	const Serialization::JSONFile& GetJSONData() const;

private:

	void ClearEntities();

private:
	EntityRegistry* m_registry;
	AssetManager* m_assetManager;
private:
	Serialization::JSONFile m_jsonData	{ Serialization::JSONFileType::Object };
	SparseSet<EntityID, EntityNode> m_sceneEntities;
	bool m_isDirty{};

};


using SceneHandle = ResourceHandleT<Scene>;
