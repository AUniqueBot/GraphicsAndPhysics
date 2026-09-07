#include <algorithm>
#include <arch/resources/res_scene.h>
#include <arch/ecs/ecs_registry.h>
#include <arch/resources/res_assetManager.h>
#include <arch/core.h>


bool Scene::AddEntity(EntityID _id) {
	if (_id == EntityConstants::C_ENTITYID_INVALID) return false;

	return true;
}

EntityView Scene::Instantiate(RES_ID _prefab) {
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
	EntityID id = entitynode.m_entityId;
	m_sceneEntities.Add(std::move(entitynode), id);
	//if (_prefab != EntityConstants::C_ENTITYID_INVALID)  { Get the resource, looks like a scene file. }

	return entity;
}

bool Scene::Destroy(EntityView _entity, bool _recursive) {
	if (!_entity) return false;
	return Destroy(_entity->GetID(), _recursive);
}

bool Scene::Destroy(EntityID _id, bool _recursive) {
	// dfs removal
	auto entitynode = m_sceneEntities.At(_id);
	if (!entitynode) return false;
	for (EntityID child : entitynode->m_children) {
		Destroy(child, true);
	}
	m_registry->Destroy(_id);
	return m_sceneEntities.Remove(_id);
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

EntityRegistry* Scene::GetRegistry() {
	return m_registry;
}

const EntityRegistry* Scene::GetRegistry() const {
	return m_registry;
}

void Scene::SetRegistry(EntityRegistry* _registry) {
	m_registry = _registry;
}



AssetManager* Scene::GetAssetManager() {
	return m_assetManager;
}

const AssetManager* Scene::GetAssetManager() const {
	return m_assetManager;
}

void Scene::SetAssetManager(AssetManager* _registry) {
	m_assetManager = _registry;
}





void Scene::SetSceneJSON(const Serialization::JSONFile& _jsonData) {
	rapidjson::Document& doc = m_jsonFile.GetDocument();
	doc.CopyFrom(_jsonData.GetDocument(), doc.GetAllocator());
}

void Scene::Load() {
	// structured data to form relations with.
	struct EntityRelations {
		EntityIDType originalId			{ EntityConstants::C_ENTITYID_INVALID };
		EntityIDType originalParentId	{ EntityConstants::C_ENTITYID_INVALID };
		EntityView entity				{ std::nullopt };
	};

	if (m_jsonFile.HasMember("environment")) {
		// do environment loading here.
		
	}



	std::unordered_map<EntityIDType, EntityRelations> mappings;
	// -- pass 1 -----------------------
	if (m_jsonFile.HasMember("entities")) {
		const rapidjson::Value& entities = m_jsonFile.GetMember("entities");
		for (auto itr = entities.MemberBegin(); itr != entities.MemberEnd(); ++itr) {
			EntityRelations relationData;

			EntityIDType originalId{ std::stoul(itr->name.GetString()) };
			relationData.originalId = originalId;

			


			// props.
			const rapidjson::Value& entityprops = itr->value;
			
			
			EntityView entity = Instantiate();
			if (!entity) continue;

			relationData.entity = entity;

			if (entityprops.HasMember("name")) {
				entity->Name(entityprops["name"].GetString());
			}

			if (entityprops.HasMember("parent")) {
				relationData.originalParentId = entityprops["parent"].GetUint();
			}
			



			// instantiate and generate.
			if (entityprops.HasMember("components")) {
				const rapidjson::Value& compdata = entityprops["components"];
				for (auto comp = compdata.MemberBegin(); comp != compdata.MemberEnd(); ++comp) {
					// name -> compid -> compid -> type
					auto comptypeid = m_registry->GetCompTypeID(comp->name.GetString());
					if (!comptypeid) continue;

					// get the deserialize function here.
					ComponentPackedData& packedData = m_registry->GetComponentData(*comptypeid);
					DeserializeFunction deserializeFunction = packedData.m_componentMetadata.GetDeserializeFunction();
					if (!deserializeFunction) continue;
					Serialization::JSONFile jsonFile;
					auto& doc = jsonFile.GetDocument();
					doc.CopyFrom(comp->value, doc.GetAllocator());
					deserializeFunction(*entity, jsonFile, *m_assetManager);
					
				}
			}
			mappings[originalId] = relationData;
			
		}


		// -- parenting to new ids ---
		for (auto& [originalId, relationData] : mappings) {
			if (relationData.originalParentId != EntityConstants::C_ENTITYID_INVALID) {
				EntityIDType newId = relationData.originalId;
				EntityIDType parentId = mappings.at(relationData.originalParentId).originalId;
				Parent(newId, parentId);
			}
		}
	}

}

std::shared_ptr<Scene> Scene::LoadScene(
	const Serialization::JSONFile& _jsonData, 
	EntityRegistry& _registry, 
	AssetManager& _asMgr
) {
	auto newScene = std::make_shared<Scene>();
	newScene->SetRegistry(&_registry);
	newScene->SetAssetManager(&_asMgr);
	newScene->SetSceneJSON(_jsonData);
	return newScene;
}


void Scene::ClearEntities() {
	m_sceneEntities.clear();
	m_registry->ClearEntitiesAndComponentData();
}
