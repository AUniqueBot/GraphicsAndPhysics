#pragma once
#include <pch.h>
#include <arch/common/system.h>
#include <arch/systems/sys_render_modules/sys_render_viewportManager.h>
#include <arch/systems/sys_render_modules/sys_render_uboManager.h>
#include <arch/components/comp_light.h>


constexpr unsigned C_MAX_LIGHT_COUNT_LOW	{ 20 };
constexpr unsigned C_MAX_LIGHT_COUNT_MED	{ 40 };
constexpr unsigned C_MAX_LIGHT_COUNT_HIGH	{ 60 };


constexpr unsigned CURRENT_MAX_LIGHT_COUNT	{ 20 };

class RenderSystem final : public System, public Singleton<RenderSystem> {
public:
	
	// system funcs
	void Init()			override;


	void Start()		override { LOG_INFO("Start"); };


	void PreUpdate()	override;
	void Update()		override;
	void Stop()			override { LOG_INFO("Stop"); };
	void Cleanup()		override { LOG_INFO("Cleanup"); };



	// run the logic to render one of the cameras.
	void Render(const Viewport& _viewport);


private:
	std::vector<LightData> CullLights(const Viewport& _viewport);


	bool LightCollisionTest(const Light& _lightComponent, const Viewport& _viewport) const;
	bool SpotLightCollisionTest(const Light& _lightComponent, const Viewport& _viewport) const;
	bool PointLightCollisionTest(const Light& _lightComponent, const Viewport& _viewport) const;


private:
	const unsigned m_maxLightCount			{ C_MAX_LIGHT_COUNT_LOW };
	
	ViewportManager m_viewportManager;
	UBOManager m_uboManager;

};



