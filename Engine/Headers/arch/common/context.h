#pragma once

#include <pch.h>

#include <arch/ecs/ecs_registry.h>
#include <arch/resources/res_assetManager.h>
#include <arch/resources/res_resourceManager.h>
#include <graphics/device/gfx_gpuresourcemanager.h>



struct ApplicationContext {

	EntityRegistry& m_entityRegistry;



	ResourceManager& m_resourceManager;
	GPUResorceManager& m_gpuResourceManager;
};