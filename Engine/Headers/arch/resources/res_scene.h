#pragma once
#include <pch.h>
#include <arch/common/entityid.h>
#include <arch/resources/res_resource.h>
#include <serialization/serialize_jsonfile.h>

// only responsible for holding a bunch of references to a scene.


class Scene : public Resource<Scene> {
public:

	bool LoadScene(std::filesystem::path _path);
	bool AddEntity(EntityID _id);
	bool RemoveEntity(EntityID _id);


private:
	void EnsureUnique();
	void ClearEntities();
private:
	std::deque<EntityID> m_entities;

};