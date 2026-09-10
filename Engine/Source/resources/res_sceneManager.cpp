#include <filesystem>
#include <arch/resources/res_sceneManager.h>
#include <serialization/serialize_metafilereader.h>
#include <serialization/serialize_jsonfile.h>
#include <arch/ecs/ecs_registry.h>

void SceneManager::Init() {
	RegisterFileExtension(".scene");
	

	using namespace Serialization;
	auto metadata = MetafileSerializer::ScanForMetafilesInPath("./Assets");
	for (const MetafileData& metafile : metadata) {
		if (AcceptsFileExtension(metafile.path.extension().string())) {
			LoadResource(metafile);
		}
	}


}




void SceneManager::CreateScene() {
	// makes a new scene and clears EVERYTHING.
	auto scene = std::make_shared<Scene>();
	SceneHandle handle(m_resourceManager.AddInternalResource(scene));
	AddResourceToPool(handle);
	ClearScene();
	// set this handle as the current scene
	m_currentScene = handle; 
}

void SceneManager::LoadCurrentScene() {
	if (m_resourceIdPool.contains(m_currentScene->ResourceID())) {
		m_currentScene->Load();
	}
}

void SceneManager::SetCurrentScene(RES_ID _sceneId) {
	SceneHandle handle(m_resourceManager.GetResourceIdentifier(_sceneId));
	SetCurrentScene(handle);
}

void SceneManager::SetCurrentScene(SceneHandle _scene) {
	if (_scene == m_currentScene) return;
	m_currentScene = _scene;
}


void SceneManager::ClearScene() {
	m_entityRegistry.ClearEntitiesAndComponentData();
}

SceneHandle SceneManager::LoadScene(const std::filesystem::path& _path, RES_ID _existingId) {
	Serialization::JSONFile json;
	json.Parse(_path);
	std::shared_ptr<Scene> scene = Scene::LoadScene(json, m_entityRegistry, m_assetManager);
	if (_existingId != ResourceConstants::C_RES_INVALID_ID) {
		scene->ResourceID(_existingId);
	}
	SceneHandle handle(m_resourceManager.AddInternalResource(scene));
	return handle;
}

void SceneManager::SaveScene(const std::filesystem::path& _path) {
	Serialization::JSONFile json(Serialization::JSONFileType::Object);

	rapidjson::Value envVal;
	json.AddMember("environment", envVal);

	rapidjson::Value entities;
	json.AddMember("entities", entities);

}


void SceneManager::LoadResource(const Serialization::MetafileData& _data) {
	namespace fs = std::filesystem;
	fs::path path = _data.path;
	if (!fs::exists(_data.path)) {
		LOG_WARN("Unable to open resource: " << _data.path);
		return;
	}
	SceneHandle handle = LoadScene(path, _data.id);
	handle->ResourcePath(path);
	AddResourceToPool(handle);
}

