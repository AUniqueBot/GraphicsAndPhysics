#include <arch/ecs/ecs_entityFactory.h>
#include <arch/components/comp_transform.h>

#include <arch/components/comp_meshrenderer.h>
#include <arch/resources/res_mesh_presets/res_mesh_cube.h>
#include <arch/resources/res_mesh_presets/res_mesh_plane.h>


EntityView EntityFactory::CreateCube(const glm::vec3& pos) {
	// TODO: insert return statement here

	EntityView objv{ m_registry.Instantiate() } ;

	if (objv) {
		Entity& e{*objv};
		e.Name("Cube");
		e.AddComponent<MeshRenderer>();
		ComponentView<MeshRenderer> mr	{ e.GetComponent<MeshRenderer>() };
		if (mr) {
			mr->SetMesh(std::make_shared<Cube>(Cube()));
			mr->GetMesh()->Init();
			mr->Init();
		}
		e.GetComponent<Transform>()->Position(pos);
	}
	return objv;
}

EntityView EntityFactory::CreateSphere(const glm::vec3& pos) {
	// TODO: insert return statement here
	EntityView objv{ m_registry.Instantiate() };

	if (objv) {
		Entity& e{ *objv };
		e.Name("Sphere");
		e.AddComponent<MeshRenderer>();
		ComponentView<MeshRenderer> mr{ e.GetComponent<MeshRenderer>() };
		if (mr) {
			mr->SetMesh(std::make_shared<Cube>(Cube()));
			mr->GetMesh()->Init();
			mr->Init();
		}
		e.GetComponent<Transform>()->Position(pos);
	}
	return objv;
}

EntityView EntityFactory::CreatePlane(const glm::vec3& pos) {
	// TODO: insert return statement here
	EntityView objv{ m_registry.Instantiate() };

	if (objv) {
		Entity& e{ *objv };
		e.Name("Plane");
		e.AddComponent<MeshRenderer>();
		ComponentView<MeshRenderer> mr{ e.GetComponent<MeshRenderer>() };
		if (mr) {
			mr->SetMesh(std::make_shared<Plane>(Plane()));
			mr->GetMesh()->Init();
			mr->Init();
		}
		e.GetComponent<Transform>()->Position(pos);
	}
	return objv;
}
