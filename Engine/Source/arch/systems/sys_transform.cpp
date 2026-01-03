#include <pch.h>
#include <arch/systems/sys_transform.h>
// ecs
#include <arch/core.h>
#include <arch/ecs/ecs_fwdDecl_entityRegistry.h>
// components
#include <arch/components/comp_meshrenderer.h>
#include <arch/components/comp_camera.h>
#include <arch/components/comp_transform.h>
#include <util/util_print.h>


void TransformSystem::Start() {
	//LOG_INFO("Start");
}

void TransformSystem::Update() {
	EntityRegistry& registry = Core::GetInstance().Registry();
	auto data = registry.GetComponentPool<Transform>();
	auto& compPool = *(data);
}
