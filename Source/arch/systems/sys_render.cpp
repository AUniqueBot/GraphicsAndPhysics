#include <pch.h>
#include <arch/systems/sys_render.h>
// ecs
#include <arch/core.h>
#include <arch/ecs/ecs_fwdDecl_entityRegistry.h>
// components
#include <arch/components/comp_meshrenderer.h>
#include <arch/components/comp_camera.h>
#include <arch/components/comp_transform.h>`
#include <util/util_ostreamOverrides.h>
#include <util/util_logging.h>



// Some other todos - make a shader editor! How hard can it be? :')

void RenderSystem::Init() {

	// - initialise gl settings -----------------
	glEnable(GL_DEPTH_TEST);



	// - init viewport manager ------------------

	Viewport::ViewportID vpId	{ m_viewportManager.CreateViewport() };
	Viewport& viewport			{ m_viewportManager.ViewportList().at(vpId)	};

	

	LOG_INFO("Run Init");


}

void RenderSystem::PreUpdate() {
	// clear the buffer.
	//LOG_INFO("Run PreUpdate");
}


void RenderSystem::Update() {
	glm::mat4 dummy{};
	/*
		disable
		clear screen
		
		
		for each viewport

		// - viewport setup ----------------------------
		get the size of each viewport 
		get the position of each viewport

		// - render setup ------------------------------
		get the camera matrix
		get the projection matrix
		render();
	
	
	*/
	GLuint clearFlags{ GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT };



	const std::vector<Viewport::ViewportID>& vpRenderOrder	{ m_viewportManager.ViewportRenderOrderList() };
	auto& viewportMap					{ m_viewportManager.ViewportList() };

	glDisable(GL_SCISSOR_TEST);
	glClearColor(0, 0, 0, 1);
	glClear(clearFlags);

	glEnable(GL_SCISSOR_TEST); // assuming the engine doesn't need overlays. then agin, can be alleviated with a render order test.
	for (const Viewport::ViewportID& id : vpRenderOrder) {
		Viewport& currentViewport	{ viewportMap.at(id ) };


		currentViewport.Update();


		glClearColor(0.39f, 0.58f, 0.93f, 1.0f);
		glClear(clearFlags);
		Render(currentViewport); // replace with a single viewport.
	}	
}





void RenderSystem::Render(const Viewport& _viewport) {
	const glm::mat4& _cameraMatrix			{ glm::inverse(_viewport.CameraMatrix()) };
	const glm::mat4 & _projectionMatrix		{ _viewport.ProjectionMatrix() };;

	EntityRegistry& registry = Core::GetInstance().Registry();
	// use the current camera for projection matrix.

	// set to wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	glm::mat4 pos, rot, scl{ 1.f };
	glm::mat4 viewMtx{ 1.f };
	pos = glm::translate(glm::mat4{ 1.f }, glm::vec3());
	rot = glm::mat4{ 1.f };

	

	std::vector<LightData> lightList = CullLights(_viewport);
	// set into UBO here.
	



	auto& entityList = registry.GetEntityList();
	for (Entity& e : entityList) {
		const auto& mr = e.GetComponent<MeshRenderer>();
		if (!mr) {
			continue;
		}
		
		

		auto matHandle = 0;	// pls get the material now.
		auto trs = e.GetComponent<Transform>();

		// assigns this buffer to mesh.
		const Mesh& mesh = mr->GetMesh();
		mesh.UseVAO();

		if (false) {
			// go through each material
			glUseProgram(mr->GetMaterialList()[0].GetShader());
		}
		else {
			const Material& mat = mr->GetDefaultMaterial();
			GLuint program = mr->GetDefaultMaterial().GetShader();
			glUseProgram(program);	
			glm::mat4 objMat{ 1.f };

			static glm::vec3 rot_v	{};
			double deltaTime		{ Core::DeltaTime() };


			rot_v[0] += static_cast<float>(deltaTime * 50);
			rot_v[1] += static_cast<float>(deltaTime * 50);
			rot_v[2] += static_cast<float>(deltaTime * 50);

			trs->RotationEuler(rot_v);
			pos = glm::translate(objMat, trs->Position());
			rot = glm::mat4(trs->Rotation());
			scl = glm::scale(objMat, trs->Scale());
			objMat = pos * rot * scl;

			mat.Render(
				objMat,
				_projectionMatrix,
				_cameraMatrix
			);
		}

		// for now, position the frag shader
		glDrawElements(GL_TRIANGLES, mesh.IndexCount(), GL_UNSIGNED_INT, 0);

		

	}
	glBindVertexArray(0);
}



std::vector<LightData> RenderSystem::CullLights(const Viewport& _viewport) {

	std::vector<LightData> potentialLights{};

	EntityRegistry& registry = Core::GetInstance().Registry();
	const auto& lightPoolRef = registry.GetComponentPool<Light>();

	if (!registry.ComponentPoolExists<Light>()) {
		LOG_WARN("Light Component Pool does not exist");
		return std::vector<LightData> {};
	}


	const ComponentPool<Light>& lightPool = *lightPoolRef;
	const unsigned lightCount = m_maxLightCount;
	const auto& lightComponentData = lightPool.Data();

		
	for (const auto& light : lightComponentData) {

		EntityView entity = registry.Get(light.GetEntityID());
		if (entity) continue;

		//
		if (!LightCollisionTest(light, _viewport)) {
			// light does not appear or cause an effect in the camera frustum.
			continue;
		}

		ComponentView<Transform> trs = entity->GetComponent<Transform>();
		
		LightData lightData = light.GetLightData();
		glm::vec3 position = trs->Position();
		lightData.SetPosition(position);
		LOG_INFO("Position: " << position);
		potentialLights.push_back(lightData);
	}
	return potentialLights;
}



bool RenderSystem::LightCollisionTest(const Light& _lightComponent, const Viewport& _viewport) const {
	bool testCase = false;
	switch (_lightComponent.Type()){
	case POINT:
		testCase = PointLightCollisionTest(_lightComponent, _viewport);
		break;
	case AREA:
		break;
	case SPOT:
		testCase = SpotLightCollisionTest(_lightComponent, _viewport);
		break;
	case DIRECTIONAL:
		break;
	case AMBIENT:
		testCase = true;
		break;
	case SUN:
		testCase = true;
		break;

	default:
		break;
	}
	return testCase;
}

bool RenderSystem::SpotLightCollisionTest(const Light& _lightComponent, const Viewport& _viewport) const {
	(void)_lightComponent;
	return true;
}

bool RenderSystem::PointLightCollisionTest(const Light& _lightComponent, const Viewport& _viewport) const {
	(void)_lightComponent;
	return true;
}


