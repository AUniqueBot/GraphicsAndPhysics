#pragma once
#include <unordered_set>
#include <arch/resources/res_scene.h>
#include <arch/datatypes/type_sparseSet.h>
#include <arch/resources/res_specializedResourceManager.h>


namespace SceneConstants {
	inline constexpr const char C_SCENE_DEFAULT[]		 { "C_SCENE_DEFAULT" };
}


// resource manager not under asset manager
class SceneManager : public SpecializedResourceManager {
public:
	enum SceneState {
		Pause,
		Play,
		Stop,
		Loading
	};
public:
	SceneManager(ResourceManager& _rsm, GPUResourceManager& _gpuRsm, EntityRegistry& _er, AssetManager& _asMgr) : 
		SpecializedResourceManager(_rsm, _gpuRsm), m_entityRegistry{ _er }, m_assetManager{ _asMgr } {
	}
	
	void Init() override;
	
public:
	void CreateScene();
	void DeleteScene();

	void LoadCurrentScene();
	void SetCurrentScene(RES_ID _sceneId);
	void SetCurrentScene(SceneHandle _scene);

	SceneHandle& GetCurrentScene();
	const SceneHandle& GetCurrentScene() const;


	void ClearScene();

	SceneHandle LoadScene(const std::filesystem::path& _path, RES_ID _existingId = ResourceConstants::C_RES_INVALID_ID);
	void SaveScene(const std::filesystem::path& _path = "");
public:
	void LoadResource(const Serialization::MetafileData& _data) override;
private:
	SceneState m_currentState				{ Stop };
	SceneHandle m_currentScene				{ std::nullopt };
	EntityRegistry& m_entityRegistry;
	AssetManager& m_assetManager;
};
