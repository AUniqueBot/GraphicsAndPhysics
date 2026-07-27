#pragma once
#include <arch/common/scene.h>
#include <arch/datatypes/type_sparseSet.h>



class SceneManager {
public:
	enum class SceneLoadingState {
		LOADING,	// in the process of loading
		LOADED,		// a scene is loaded
		UNLOADING,	// a scene is being unloaded
		UNLOADED	// no scene is currently loaded
	};


public:

	void LoadScene(const Scene& _scene);
	void SaveScene();

private:
	void Clear();

private:
	Scene m_currentScene	{};
	
};