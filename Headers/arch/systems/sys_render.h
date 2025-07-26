#pragma once
#include <pch.h>
#include <arch/common/system.h>
#include <arch/systems/sys_render_modules/sys_render_viewportManager.h>

#include <arch/components/comp_light.h>


constexpr unsigned C_MAX_LIGHT_COUNT_LOW	{ 20 };
constexpr unsigned C_MAX_LIGHT_COUNT_MED	{ 40 };
constexpr unsigned C_MAX_LIGHT_COUNT_HIGH	{ 60 };

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
	void Render(const glm::mat4& _cameraMatrix, const glm::mat4&  _projectionMatrix);


private:
	bool LightCollisionTest(const Light& _lightComponent);
	bool SpotLightCollisionTest(const Light& _lightComponent);
	bool PointLightCollisionTest(const Light& _lightComponent);


private:
	const unsigned m_maxLightCount			{ C_MAX_LIGHT_COUNT_LOW };
	ViewportManager m_viewportManager;

};



