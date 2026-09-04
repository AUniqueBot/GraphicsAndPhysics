#pragma once
#include <unordered_set>
#include <arch/resources/res_scene.h>
#include <arch/datatypes/type_sparseSet.h>
#include <arch/resources/res_specializedResourceManager.h>

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
	SceneManager(ResourceManager& _rsm, GPUResourceManager& _gpuRsm) : 
		SpecializedResourceManager(_rsm, _gpuRsm) {
	}
	
	void CreateScene();
	void DeleteScene();
	void LoadScene(const std::filesystem::path& _path);
	void SaveScene(const std::filesystem::path& _path);
private:
	RES_ID m_currentScene					{ ResourceConstants::C_RES_INVALID_ID };
};
