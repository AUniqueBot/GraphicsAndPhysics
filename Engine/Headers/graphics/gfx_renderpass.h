#pragma once
#include <pch.h>

#include <graphics/gfx_graphicscontext.h>

#include <arch/common/entityid.h>
#include <arch/components/comp_transform.h>
#include <arch/components/comp_light.h>
#include <arch/components/comp_meshRenderer.h>



template<typename T>
concept RenderPass = requires(T & pass) {
	&T::Execute;
};


// struct
struct MeshRenderable {
	EntityID id;
	Entity& entity;
	ComponentView<Transform> transform;
	ComponentView<MeshRenderer> mesh;
};

struct LightRenderable {
	EntityID id;
	Entity& entity;
	ComponentView<Transform> transform;
	ComponentView<Light> light;
};
struct SceneRenderData {
	std::vector<MeshRenderable> meshes;
	std::vector<LightRenderable> lights;
};