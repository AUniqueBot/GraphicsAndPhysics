#pragma once


class Scene;


class SceneManager {
public:
	enum class LoadState {
		LOADING,	// in the process of loading a scene
		LOADED,		// a scene is loaded
		UNLOADING,	// a scene is being unloaded
		UNLOADED	// no scene is currently loaded
	};


public:
	void LoadScene(std::shared_ptr<Scene> _scene);
	void UnloadScene();
	void SaveScene();

	LoadState GetCurrentLoadState();
	

private:
	void Clear();

private:
	std::shared_ptr<Scene> m_currentScene{};
	LoadState m_currentState	{ LoadState::UNLOADED };
	LoadState m_nextState		{ LoadState::UNLOADED };
};