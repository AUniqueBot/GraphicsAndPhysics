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
	glm::vec3 colour	{};
	float power			{};
		
};


void RenderSystem::Init() {
	std::cout << typeid(RenderSystem).name() << "::Init" << std::endl; 
}

void RenderSystem::PreUpdate() {
	// clear the buffer.


}



void RenderSystem::Update() {

	EntityRegistry& registry = Core::Registry();
	
	for (Entity& e : registry.GetEntityList()) {
	

		// render a mesh.
		if (e.GetComponent<MeshRenderer>().has_value()) {
			auto meshHandle = e.GetComponent<MeshRenderer>();
			auto trsHandle = e.GetComponent<Transform>();
			MeshRenderer& mr	= meshHandle.value().get();
			Transform& trs		= trsHandle.value().get();


			// to dos - some sort of pointer from the component to entityID.
			// every entity should have a transform component attached.
		

			LightData ld;
			float sdsq	{ -1 };
			for (Entity& e2 : registry.GetEntityList()) {
				auto lightHandle = e2.GetComponent<Light>();
				if (!lightHandle.has_value()) continue;
				Light& lightComp = lightHandle.value().get();



				glm::vec3 pos = e2.GetComponent<Transform>().value().get().Position();

				float dist = glm::distance(trs.Position(), pos);
				if (sdsq < 0 || sdsq > dist) {
					ld.position = pos;
					ld.power = lightComp.Power();
					ld.colour = lightComp.Colour();
				}

			}
			if (sdsq == -1) continue;


			// assigns this buffer to mesh.
			mr.GetMesh().UseVAO();

			// if no light data continue (sdsq == -1)






		}

	}

}
