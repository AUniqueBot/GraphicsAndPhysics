#include <filesystem>
#include <arch/resources/res_sceneManager.h>
#include <serialization/serialize_metafilereader.h>
#include <serialization/serialize_jsonfile.h>


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

}

void SceneManager::LoadCurrentScene() {
	if (m_resourceIdPool.contains(m_currentScene)) {
		auto ptr = static_pointer_cast<Scene>(m_resourceManager.GetResource(m_currentScene));
		ptr->Load();
	}
}

void SceneManager::SetCurrentScene(RES_ID _sceneId) {
	if (_sceneId == m_currentScene ) return;
	m_currentScene = _sceneId;
	// set load state. async later?
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

