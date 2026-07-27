#include <arch/core/core_scenemanager.h>
#include <arch/core.h>

void SceneManager::LoadScene(const Scene& _scene) {
	Clear();
	m_currentScene = _scene;
	
}

void SceneManager::SaveScene() {

}

void SceneManager::Clear() {
	Core& c = Core::GetInstance();
	c.GetRegistry().ClearEntitiesAndComponentData();
}
