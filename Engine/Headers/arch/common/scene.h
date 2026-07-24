#pragma once
#include <pch.h>
#include <arch/common/entity.h>

class Scene {
public:
	Scene(EntityRegistry* _registry);
	bool AddEntity(EntityID _id);
	bool RemoveEntity(EntityID _id);
	
	
private:
	void EnsureUnique();
	void ClearEntities();
private:
	
	EntityRegistry* m_entityRegistry;
	std::deque<EntityID> m_entities;
	

};