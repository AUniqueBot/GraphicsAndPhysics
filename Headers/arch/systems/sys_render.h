#pragma once
#include <pch.h>
#include <arch/common/system.h>
#include "./sys_render_modules/sys_render_viewportManager.h"


class RenderSystem final : public System, public Singleton<RenderSystem> {
public:
	
	// system funcs
	void Init()		override;


	void Start()	override { LOG_INFO("Start"); };


	void PreUpdate() override;
	void Update()	override;
	void Stop()		override { LOG_INFO("Stop"); };
	void Cleanup()	override { LOG_INFO("Cleanup"); };




	// run the logic to render one of the cameras.
	void Render(const glm::mat4& _cameraMatrix, const glm::mat4&  _projectionMatrix);

	// coordinate systems

private:

	ViewportManager m_viewportManager;


};



