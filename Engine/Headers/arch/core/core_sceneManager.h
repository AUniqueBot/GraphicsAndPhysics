#pragma once
#include <arch/common/scene.h>
#include <arch/datatypes/type_sparseSet.h>
#include <arch/common/scene.h>

class SceneManager {
	void LoadScene(const Scene& _scene);
	void SaveScene();


private:
	Scene m_currentScene	{};
};