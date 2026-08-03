#include <arch/core.h>
#include <arch/core/core_scenemanager.h>



void SceneManager::LoadScene(std::shared_ptr<Scene> _scene) {
	m_currentScene = _scene;
}

void SceneManager::UnloadScene() {
	m_nextState = LoadState::UNLOADING;

}

void SceneManager::SaveScene() {

}

SceneManager::LoadState SceneManager::GetCurrentLoadState() {
	return m_currentState;
}

void SceneManager::Clear() {
	Core& c = Core::GetInstance();
	c.GetRegistry().ClearEntitiesAndComponentData();
}
