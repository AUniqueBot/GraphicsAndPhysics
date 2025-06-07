#include <pch.h>
#include <arch/systems/sys_render.h>
// ecs
#include <arch/core.h>
#include <arch/ecs/ecs_fwdDecl_entityRegistry.h>
// components
#include <arch/components/comp_meshrenderer.h>
#include <arch/components/comp_light.h>
#include <arch/components/comp_camera.h>
#include <arch/components/comp_transform.h>`

// Some other todos - make a shader editor! How hard can it be? :')

struct LightData {

	glm::vec3 position	{};
	glm::vec3 rotation	{};
	float intensity		{};
		
};


void RenderSystem::Init() {
	std::cout << typeid(RenderSystem).name() << "::Init" << std::endl; 
}

void RenderSystem::PreUpdate() {
	// clear the buffer.


}

void RenderSystem::Update() {

	EntityRegistry& registry = Core::Registry();

	auto cameraData		= registry.GetComponentPool<Camera>();
	auto meshData			= registry.GetComponentPool<MeshRenderer>();
	auto lightData		= registry.GetComponentPool<Light>();

	auto& camCompPool			= cameraData.value().get();
	auto& meshCompPool	= meshData.value().get();
	auto& lightCompPool		= lightData.value().get();
	

	if (!cameraData.has_value()) return;
	for (auto& camera : camCompPool) {


		if (!meshData.has_value()) continue;
		for (auto& mesh : meshCompPool) {

			std::vector<LightData> lightList;


			if (!lightData.has_value()) continue;
			for (auto& light : lightCompPool) {


			}
		}
	}
	

}
