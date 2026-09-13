#include <filesystem>
#include <arch/resources/res_sceneManager.h>
#include <serialization/serialize_metafilereader.h>
#include <serialization/serialize_jsonfile.h>
#include <arch/ecs/ecs_registry.h>

void SceneManager::Init() {
	RegisterFileExtension(".scene");
}


void SceneManager::Load(std::filesystem::path _path) {
	using namespace Serialization;
	auto metadata = MetafileSerializer::ScanForMetafilesInPath(_path);
	for (const MetafileData& metafile : metadata) {
		if (AcceptsFileExtension(metafile.path.extension().string())) {
			LoadResource(metafile);
		}
	}
}



void SceneManager::CreateScene() {
	// makes a new scene and clears EVERYTHING.
	auto scene = std::make_shared<Scene>();
	scene->SetRegistry(&m_entityRegistry);
	scene->SetAssetManager(&m_assetManager);
	SceneHandle handle(m_resourceManager.AddRes(scene));
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
	SceneHandle handle(m_resourceManager.AddRes(scene));
	return handle;
}


SceneHandle& SceneManager::GetCurrentScene() {
	return m_currentScene;
}
const SceneHandle& SceneManager::GetCurrentScene() const {
	return m_currentScene;
}


void SceneManager::SaveScene(const std::filesystem::path& _path) {
	if (!_path.empty() && std::filesystem::exists(_path.parent_path())) {
		m_currentScene->ResourcePath(_path);
	}

	m_currentScene->Save(); // saves to internal document
	std::filesystem::path outputPath = m_currentScene->ResourcePath();

	Serialization::SaveJSONFile(m_currentScene->GetJSONData(), outputPath);
	
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

