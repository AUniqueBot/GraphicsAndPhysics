#pragma once
#include <pch.h>
#include <arch/common/system.h>
#include "./sys_render_modules/sys_render_viewportManager.h"


class RenderSystem final : public System, public Singleton<RenderSystem> {
public:
	
	// system funcs
	void Init()		override;


	void Start()	override { std::cout << typeid(RenderSystem).name() << "::Start" << std::endl;};


	void PreUpdate() override;
	void Update()	override;
	void Stop()		override { std::cout << typeid(RenderSystem).name() << "::Stop" << std::endl; };
	void Cleanup()	override { std::cout << typeid(RenderSystem).name() << "::Cleanup" << std::endl; };




	// run the logic to render one of the cameras.
	void Render(const glm::mat4& _cameraMatrix, const glm::mat4&  _projectionMatrix);

	// coordinate systems

private:

	ViewportManager m_viewportManager;


};



