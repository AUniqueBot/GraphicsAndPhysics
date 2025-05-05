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
	 // std::cout << typeid(RenderSystem).name() << "::Update" << std::endl; 

	// rasterise all renderables 

	// once completed, print to screen.



}
