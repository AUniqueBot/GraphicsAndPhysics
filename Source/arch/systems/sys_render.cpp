#include <pch.h>
#include <arch/systems/sys_render.h>
// ecs
#include <arch/core.h>
#include <arch/ecs/ecs_fwdDecl_entityRegistry.h>
// components
#include <arch/components/comp_meshrenderer.h>
#include <arch/components/comp_camera.h>
#include <arch/components/comp_transform.h>`

// Some other todos - make a shader editor! How hard can it be? :')

void RenderSystem::Init() {
	std::cout << typeid(RenderSystem).name() << "::Init" << std::endl; 
}

void RenderSystem::PreUpdate() {
	// clear the buffer.


}

void RenderSystem::Update() {
	auto data = Core::Registry().GetComponentPool<MeshRenderer>();
	auto& compPool = data.value().get();
	

	/*
		
	*/


	for (auto& mesh : compPool) {
		// do a loop here.

		// push this mesh to buffer or something idk.

		mesh.GetMesh().Init();

		// we're assuming that 





	}

}
