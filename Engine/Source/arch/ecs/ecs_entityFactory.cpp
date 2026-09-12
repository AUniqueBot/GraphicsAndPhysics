#include <arch/ecs/ecs_entityFactory.h>
#include <arch/components/comp_transform.h>

#include <arch/components/comp_meshrenderer.h>
#include <arch/resources/res_mesh_presets/res_mesh_types.h>

#include <arch/components/comp_light.h>

#include <arch/resources/res_material_presets/res_material_lambert.h>




EntityView EntityFactory::CreateGameObject(const glm::vec3& pos) {
	SceneHandle scene = m_sceneManager.GetCurrentScene();

	EntityView objv{ scene->Instantiate() };
	if (objv) {
		Entity& e{ *objv };
		e.Name("gameObject");
		e.GetComponent<Transform>()->Position(pos);
	}
	return objv;
}


EntityView EntityFactory::CreateCube(const glm::vec3& pos) {
	// TODO: insert return statement here
	SceneHandle scene = m_sceneManager.GetCurrentScene();
	EntityView objv{ scene->Instantiate() } ;

	if (objv) {
		Entity& e{*objv};
		e.Name("Cube");
		e.AddComponent<MeshRenderer>();
		ComponentView<MeshRenderer> mr	{ e.GetComponent<MeshRenderer>() };

		if (mr) {
			CubeHandle mesh = m_assetManager.GetMeshManager().CreateCubeMesh();
			mr->SetMesh(mesh);
			mr->Init();
		}
		e.GetComponent<Transform>()->Position(pos);
	}
	return objv;
}

EntityView EntityFactory::CreateSphere(const glm::vec3& pos) {
	// TODO: insert return statement here
	SceneHandle scene = m_sceneManager.GetCurrentScene();
	EntityView objv{ scene->Instantiate() };

	if (objv) {
		Entity& e{ *objv };
		e.Name("Sphere");
		e.AddComponent<MeshRenderer>();
		ComponentView<MeshRenderer> mr{ e.GetComponent<MeshRenderer>() };
		if (mr) {
			SphereHandle mesh = m_assetManager.GetMeshManager().CreateSphereMesh();
			mr->SetMesh(mesh);
			mr->Init();
		}
		e.GetComponent<Transform>()->Position(pos);
	}
	return objv;
}

EntityView EntityFactory::CreatePlane(const glm::vec3& pos) {
	// TODO: insert return statement here
	SceneHandle scene = m_sceneManager.GetCurrentScene();
	EntityView objv{ scene->Instantiate() };

	if (objv) {
		Entity& e{ *objv };
		e.Name("Plane");
		e.AddComponent<MeshRenderer>();
		ComponentView<MeshRenderer> mr{ e.GetComponent<MeshRenderer>() };
		if (mr) {
			PlaneHandle mesh = m_assetManager.GetMeshManager().CreatePlaneMesh();
			mr->SetMesh(mesh);
			mr->Init();
		}
		e.GetComponent<Transform>()->Position(pos);
	}
	return objv;
}

EntityView EntityFactory::CreateDefaultCube(const glm::vec3& _pos) {	
	SceneHandle scene = m_sceneManager.GetCurrentScene();
	EntityView objv{ scene->Instantiate() };

	if (objv) {
		Entity& e{ *objv };
		e.Name("Default Cube");
		e.AddComponent<MeshRenderer>();
		ComponentView<MeshRenderer> mr{ e.GetComponent<MeshRenderer>() };
		if (mr) {
			CubeHandle mesh = m_assetManager.GetMeshManager().GetDefaultCubeMesh();
			mr->SetMesh(mesh);
			mr->Init();
		}
		e.GetComponent<Transform>()->Position(_pos);
	}
	return objv;
}

EntityView EntityFactory::CreateDefaultSphere(const glm::vec3& _pos) {
	SceneHandle scene = m_sceneManager.GetCurrentScene();
	EntityView objv{ scene->Instantiate() };

	if (objv) {
		Entity& e{ *objv };
		e.Name("Default Sphere");
		e.AddComponent<MeshRenderer>();
		ComponentView<MeshRenderer> mr{ e.GetComponent<MeshRenderer>() };
		if (mr) {
			SphereHandle mesh = m_assetManager.GetMeshManager().GetDefaultSphereMesh();
			mr->SetMesh(mesh);
			mr->Init();
		}
		e.GetComponent<Transform>()->Position(_pos);
	}
	return objv;
}

EntityView EntityFactory::CreateDefaultPlane(const glm::vec3& _pos) {
	SceneHandle scene = m_sceneManager.GetCurrentScene();
	EntityView objv{ scene->Instantiate() };

	if (objv) {
		Entity& e{ *objv };
		e.Name("Default Plane");
		e.AddComponent<MeshRenderer>();
		ComponentView<MeshRenderer> mr{ e.GetComponent<MeshRenderer>() };
		if (mr) {
			PlaneHandle mesh = m_assetManager.GetMeshManager().GetDefaultPlaneMesh();
			mr->SetMesh(mesh);
			mr->Init();
		}
		e.GetComponent<Transform>()->Position(_pos);
	}
	return objv;
}



EntityView EntityFactory::CreatePointLight(
	const glm::vec3& pos,
	const float& _intensity,
	const glm::vec4& _col
) {
	SceneHandle scene = m_sceneManager.GetCurrentScene();
	EntityView objv{ scene->Instantiate() };

	if (objv) {
		Entity& e{ *objv };
		e.Name("Point Light");
		e.AddComponent<Light>();
		ComponentView<Light> light{ e.GetComponent<Light>() };
		if (light) {
			light->Power(_intensity);
			light->SetColor(_col);
			light->Type(LightType::POINT);
		}
		e.GetComponent<Transform>()->Position(pos);
	}
	return objv;
}

EntityView EntityFactory::CreateAmbientLight(
	const glm::vec3& pos,
	const float& _intensity,
	const glm::vec4& _col
) {
	SceneHandle scene = m_sceneManager.GetCurrentScene();
	EntityView objv{ scene->Instantiate() };

	if (objv) {
		Entity& e{ *objv };
		e.Name("Ambient Light");
		e.AddComponent<Light>();
		ComponentView<Light> light{ e.GetComponent<Light>() };
		if (light) {
			light->Power(_intensity);
			light->SetColor(_col);
			light->Type(LightType::AMBIENT);
		}
		e.GetComponent<Transform>()->Position(pos);
	}
	return objv;
}

EntityView EntityFactory::CreateDirectionalLight(
	const glm::vec3& pos,
	const float& _intensity,
	const glm::vec4& _col
) {
	SceneHandle scene = m_sceneManager.GetCurrentScene();
	EntityView objv{ scene->Instantiate() };

	if (objv) {
		Entity& e{ *objv };
		e.Name("Directional Light");
		e.AddComponent<Light>();
		ComponentView<Light> light{ e.GetComponent<Light>() };
		if (light) {
			light->Power(_intensity);
			light->SetColor(_col);
			light->Type(LightType::DIRECTIONAL);
		}
		e.GetComponent<Transform>()->Position(pos);
	}
	return objv;
}