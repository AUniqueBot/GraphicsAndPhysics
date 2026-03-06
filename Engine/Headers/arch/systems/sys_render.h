#pragma once
#include <pch.h>
#include <arch/common/system.h>
#include <arch/systems/sys_render_modules/sys_render_renderTargetManager.h>
#include <arch/systems/sys_render_modules/sys_render_viewportManager.h>
#include <arch/systems/sys_render_modules/sys_render_compositor.h>
#include <arch/systems/sys_render_modules/sys_render_uboManager.h>
#include <arch/systems/sys_render_modules/sys_render_vaoManager.h>
#include <arch/systems/sys_render_modules/sys_render_shadowMap.h>
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
	void Cleanup()		override { m_shadowMap.Destroy(); LOG_INFO("Cleaning up shadow map");  };


	Viewport::RENDERMODE GetRenderMode() const;
	void SetRenderMode(Viewport::RENDERMODE _renderMode);

	Viewport::FACETORENDER GetFaceRenderedDirection() const;
	void SetFaceRenderedDirection(Viewport::FACETORENDER _setting);

	// run the logic to render one of the cameras.
	void Render(const Viewport& _viewport);
private: 
	
private: 
	void ClearBuffers(const Viewport& _viewport);
	void UseViewport(const Viewport& _viewport);
	void UnbindViewport(const Viewport& _viewport);


	void BeginViewportPass(const Viewport& _viewport);
	void EndViewportPass(const Viewport& _viewport);
	void SetupRenderSettings(const Viewport& _viewport);




	void ShadowRenderPass(
		const Viewport& _viewport,
		const EntityRegistry& _er
	);
	void LightingRenderPass(
		const Viewport& _viewport,
		const EntityRegistry& _er
	);


public:
	// ubo
	void FillLightBufferUBO(const std::vector<LightData>& _culledLightList);
	void FillShadowMapUBO();



	void DebugRenderPass(const unsigned& );

public:


	ViewportManager& GetViewportManager();
	const ViewportManager& GetViewportManager() const;

	RenderTargetManager& GetRenderTargetManager();
	const RenderTargetManager& GetRenderTargetManager() const;

	Compositor& GetCompositor();
	const Compositor& GetCompositor() const;



private:
	/*!
		@brief checks which lights to calculate from.
	*/
	std::vector<LightData> CullLights(const Viewport& _viewport);


	bool LightCollisionTest(const Light& _lightComponent, const Viewport& _viewport) const;
	bool SpotLightCollisionTest(const Light& _lightComponent, const Viewport& _viewport) const;
	bool PointLightCollisionTest(const Light& _lightComponent, const Viewport& _viewport) const;


	void SetupShadowProgram();
	void SetupShadowBuffers();
	void PassLightingMatrices(glm::mat4 _meshMatrix, glm::mat4 _lightMatrix);
	void BindLightingProgram();
	void UnbindLightingProgram();


private:
	const unsigned m_maxLightCount			{ C_MAX_LIGHT_COUNT_LOW };
	
	// shadow handling.

	// genera


	Viewport::FACETORENDER m_facesToRender	{ Viewport::FRONT };
	Viewport::RENDERMODE m_renderMode		{ Viewport::FILL };

	GLuint m_shadowPrg						{};
	GLint m_shadowMeshLoc					{};
	GLint m_shadowLightLoc					{};

	RenderTargetManager m_renderTargetManager;
	ViewportManager m_viewportManager;
	Compositor m_compositor;
	UBOManager m_uboManager;
	VAOManager m_vaoManager;


	ShadowMap m_shadowMap;
	GLuint m_planeShader{};

};



