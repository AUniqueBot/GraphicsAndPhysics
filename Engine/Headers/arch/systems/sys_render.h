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
#include <arch/components/comp_transform.h>




constexpr unsigned C_MAX_LIGHT_COUNT_LOW	{ 20 };
constexpr unsigned C_MAX_LIGHT_COUNT_MED	{ 40 };
constexpr unsigned C_MAX_LIGHT_COUNT_HIGH	{ 60 };


constexpr unsigned CURRENT_MAX_LIGHT_COUNT	{ 20 };




struct alignas(sizeof(glm::vec4)) CommonUBOData {
	glm::mat4 m_cameraMatrix;			// 16
	glm::mat4 m_projectionMatrix;		// 32
	glm::vec3 m_cameraPosition;			
	GLint _pad1;
	glm::vec3 m_cameraForward;
	GLint _pad2;
	glm::vec3 m_cameraUp;
	GLfloat m_deltaTime;
};


class RenderSystem final : public System, public Singleton<RenderSystem> {
public:
	 

	// system funcs
	void Init()			override;


	void Start()		override { LOG_INFO("Start"); };

	 
	void PreUpdate()	override;
	void Update()		override;
	void Stop()			override { LOG_INFO("Stop"); };
	void Cleanup()		override { m_directionalShadowMaps.Destroy(); LOG_INFO("Cleaning up shadow map");  };


	Viewport::RENDERMODE GetRenderMode() const;
	void SetRenderMode(Viewport::RENDERMODE _renderMode);

	Viewport::FACETORENDER GetFaceRenderedDirection() const;
	void SetFaceRenderedDirection(Viewport::FACETORENDER _setting);

	// run the logic to render one of the cameras.
	void Render(const Viewport& _viewport);
	 
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
private: 
	void DirectionalLightShadowRenderPass(
		const Viewport& _viewport,
		const EntityRegistry& _er,
		const Light& _light,
		const ComponentPool<MeshRenderer>& _mrPool
	);

	void PointLightShadowRenderPass(
		const Viewport& _viewport,
		const EntityRegistry& _er,
		const Light& _light,
		const ComponentPool<MeshRenderer>& _mrPool
	);

	void SpotLightShadowRenderPass(
		const Viewport& _viewport,
		const EntityRegistry& _er,
		const Light& _light,
		const ComponentPool<MeshRenderer>& _mrPool
	);
private: 
	// shadow passes.



	void RenderRangedLightShadows(
		const Light& light,
		const Transform& lightTransform,
		const EntityRegistry& entityRegistry,
		const ComponentPool<MeshRenderer>& meshPool
	);
	void RenderPointLightShadows(
		const Light& light,
		const Transform& lightTransform,
		const EntityRegistry& entityRegistry,
		const ComponentPool<MeshRenderer>& meshPool
	);


public:
	// ubo
	void FillLightBufferUBO(const std::vector<LightData>& _culledLightList);
	void FillShadowMapUBO(const std::vector<ShadowData>& _culledLightList);
	void FillCommonUBO(
		const glm::mat4& _cameraMatrix,
		const glm::mat4& _projectionMatrix,
		const glm::vec3& _cameraPosition,
		const glm::vec3& _cameraForward,
		const GLfloat& _deltaTime
	);

	void FillObjectUBO(
		const Entity& entity,
		const Transform& _trs
	);


	void Render(const MeshRenderer& _mr) const;

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
	const std::vector<Light*> CullLights(const Viewport& _viewport, EntityRegistry& _er);
	void UpdateLightingData(const std::vector<Light*> lightList, const EntityRegistry& er);
	std::vector<LightData> GetLightData(const std::vector<Light*>& lightList) const;
	std::vector<ShadowData> GetShadowData(const std::vector<Light*>& lightList) const;


	bool LightCollisionTest(const Light& _lightComponent, const Viewport& _viewport) const;
	bool SpotLightCollisionTest(const Light& _lightComponent, const Viewport& _viewport) const;
	bool PointLightCollisionTest(const Light& _lightComponent, const Viewport& _viewport) const;


	void SetupShadowProgram();
	void SetupShadowBuffers();
	void PassLightingMatrices(glm::mat4 _meshMatrix, glm::mat4 _lightMatrix);
	void BindShadowShader();
	void UnbindShadowShader();


	void ResolveMeshRendererMaterials(MeshRenderer& _mr);
	void ResolveDefaultMaterial(Material& _mr);

private:
	const unsigned m_maxLightCount			{ C_MAX_LIGHT_COUNT_LOW };
	
	// shadow handling.

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

	ShadowMap m_spotLightShadowMaps;
	ShadowMap m_directionalShadowMaps;
	ShadowMap m_pointLightShadowMaps;
	GLuint m_planeShader{};


	// UBO data.
	ShadowMapUBOData m_smData		{};
	LightUBOData m_ldData			{};
	CommonUBOData m_commonUboData	{};
	ObjectUBOData m_objectUboData	{};

};



