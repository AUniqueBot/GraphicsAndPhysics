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

		glm::mat4 camMtx			{ glm::inverse(currentViewport.CameraMatrix()) };
		glm::mat4 prjMtx			{ currentViewport.ProjectionMatrix() };


		glClearColor(0.39f, 0.58f, 0.93f, 1.0f);
		glClear(clearFlags);
		Render(camMtx, prjMtx); // replace with a single viewport.
	}	
}





void RenderSystem::Render(const glm::mat4& _cameraMatrix, const glm::mat4& _projectionMatrix) {
	

	EntityRegistry& registry = Core::GetInstance().Registry();
	// use the current camera for projection matrix.

	// set to wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 10.0f);

	glm::mat4 pos, rot, scl{ 1.f };
	glm::mat4 viewMtx{ 1.f };
	pos = glm::translate(
		glm::mat4{ 1.f },
		camPos
	);
	rot = glm::mat4{ 1.f };

	const auto& lightPoolRef = registry.GetComponentPool<Light>();
	LightData lightDataArray[C_MAX_LIGHT_COUNT_LOW]{};

	auto& entityList = registry.GetEntityList();

	// get the list of lights to prep for UBO.
	if (lightPoolRef.has_value()) {
	
		const ComponentPool<Light>& lightPool = lightPoolRef.value().get();
		const unsigned lightCount = m_maxLightCount;
		const auto& lightComponentData = lightPool.Data();
		
		unsigned count{};
		for (const auto& light: lightComponentData) {
			
			if (count >= m_maxLightCount) break;
			EntityID id = light.GetEntityID();

			// safety
			if (!registry.Get(id).has_value()) {
				continue;
			}

			const std::reference_wrapper<Transform> trs = registry.Get(id).value().get().GetComponent<Transform>().value();
			LightData lightData = light.GetLightData();


			glm::vec3 position = trs.get().Position();
			glm::vec3 lookAt = trs.get().Rotation() * glm::vec3(0, 0, -1);

			lightData.m_position_type.x = position.x;
			lightData.m_position_type.y = position.y;
			lightData.m_position_type.z = position.z;
			

			LOG_INFO("Position: " << position);
		}	
	}

	



	for (Entity& e : entityList) {



		if (e.GetComponent<MeshRenderer>().has_value()) {
			auto meshHandle = e.GetComponent<MeshRenderer>();
			auto matHandle = 0;	// pls get the material now.
			auto trsHandle = e.GetComponent<Transform>();
			MeshRenderer& mr = meshHandle.value().get();
			Transform& trs = trsHandle.value().get();


			// assigns this buffer to mesh.
			Mesh& mesh = mr.GetMesh();
			mesh.UseVAO();

			if (false) {
				// go through each material
				glUseProgram(mr.GetMaterialList()[0].GetShader());
			}
			else {





				const Material& mat = mr.GetDefaultMaterial();
				GLuint program = mr.GetDefaultMaterial().GetShader();
				glUseProgram(program);

		
				glm::mat4 objMat{ 1.f };

				static glm::vec3 rot_v	{};
				double deltaTime		{ Core::DeltaTime() };


				rot_v[0] += static_cast<float>(deltaTime * 50);
				rot_v[1] += static_cast<float>(deltaTime * 50);
				rot_v[2] += static_cast<float>(deltaTime * 50);

				trs.RotationEuler(rot_v);
				pos = glm::translate(objMat, trs.Position());
				rot = glm::mat4(trs.Rotation());
				scl = glm::scale(objMat, trs.Scale());
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

	}
	glBindVertexArray(0);
}

bool RenderSystem::LightCollisionTest(const Light& _lightComponent) {
	bool testCase = false;
	switch (_lightComponent.Type()){
	case POINT:
		testCase = PointLightCollisionTest(_lightComponent);
		break;
	case AREA:
		break;
	case SPOT:
		testCase = SpotLightCollisionTest(_lightComponent);
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

bool RenderSystem::SpotLightCollisionTest(const Light& _lightComponent) {
	(void)_lightComponent;
	return false;
}

bool RenderSystem::PointLightCollisionTest(const Light& _lightComponent) {
	(void)_lightComponent;
	return false;
}
