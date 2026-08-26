#include <algorithm>
#include <arch/resources/res_scene.h>
#include <arch/ecs/ecs_registry.h>


bool Scene::LoadScene(std::filesystem::path _path) {
	if (!_path.has_filename()) {
		LOG_ERROR("Provided path is not a file, returning false... (path provided: " << _path << ")");
		return false;
	}

	//Serialization::JSONFile file(_path);

	return true;
}

bool Scene::AddEntity(EntityID _id) {
	if (_id == EntityID::ENTITYID_INVALID) return false;
	m_entities.push_back(_id);
	EnsureUnique();
	return true;
}

bool Scene::RemoveEntity(EntityID _id) {
	auto itr = std::find(m_entities.begin(), m_entities.end(), _id);
	if (itr == m_entities.end()) return false; // nothing to find


	//m_entityRegistry->Destroy(_id); // should you even care about this?

	// rotate and pop.
	std::rotate(itr, itr + 1, m_entities.end());
	m_entities.pop_back();


	return true;
}

void Scene::EnsureUnique() {
	auto itr = std::unique(m_entities.begin(), m_entities.end());
	m_entities.resize(std::distance(m_entities.begin(), itr));
}

void Scene::ClearEntities() {
	if (!m_entities.size())
		m_entities.clear();
}
