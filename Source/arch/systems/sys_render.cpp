#include <pch.h>
#include <arch/systems/sys_render.h>
// ecs
#include <arch/core.h>
#include <arch/ecs/ecs_fwdDecl_entityRegistry.h>
// components
#include <arch/components/comp_meshrenderer.h>
#include <arch/components/comp_light.h>
#include <arch/components/comp_camera.h>
#include <arch/components/comp_transform.h>`





// Some other todos - make a shader editor! How hard can it be? :')

struct LightData {

	glm::vec3 position	{};
	glm::vec3 rotation	{};
	glm::vec3 colour	{};
	float power			{};
		
};








void RenderSystem::Init() {
	std::cout << typeid(RenderSystem).name() << "::Init" << std::endl; 




}

void RenderSystem::PreUpdate() {
	// clear the buffer.


}




void RenderSystem::Update() {

	EntityRegistry& registry = Core::Registry();
	// use the current camera for projection matrix.

	// set to wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	// bg colour.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.39f, 0.58f, 0.93f, 1.0f);



	glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 10.0f);

	glm::mat4 pos, rot, scl{ 1.f };
	glm::mat4 viewMtx{ 1.f };
	pos = glm::translate(
		glm::mat4{1.f},
		camPos
	);
	rot = glm::mat4{1.f};
		
		/*glm::rotate(
		glm::mat4{ 1.f },
		static_cast<float>(0),
		glm::vec3(0.f, 1.f, 0.f)
	);*/

	viewMtx = pos * rot * scl;
	viewMtx = glm::inverse(viewMtx); // camera's position are inversely applied to apply camera view


	glm::mat4 projectionMtx{1.f};

	// projection matrix
	projectionMtx = glm::perspective(
		glm::radians(90.f), // vertical fov 
		16.f / 9.f,					// CAMERA aspect ratio
		0.0001f,					// near clip plane
		1000.f							// far clip plane.
	);

	



	for (Entity& e : registry.GetEntityList()) {
		

		// render a mesh.
		if (e.GetComponent<MeshRenderer>().has_value()) {
			auto meshHandle = e.GetComponent<MeshRenderer>();
			auto matHandle = 0;	// pls get the material now.
			auto trsHandle = e.GetComponent<Transform>();
			MeshRenderer& mr	= meshHandle.value().get();
			Transform& trs		= trsHandle.value().get();




			// assigns this buffer to mesh.
			Mesh& mesh = mr.GetMesh();
			mesh.UseVAO();
			
			if (false && mr.GetMaterialList().size()) {
				// go through each material
				glUseProgram(mr.GetMaterialList()[0].GetShader());
			}
			else {

				GLuint program = mr.GetDefaultMaterial().GetShader();
				glUseProgram(program);
				glm::mat4 objMat	{ 1.f };
				

				pos = glm::translate(objMat, trs.Position());
				rot = glm::mat4(trs.Rotation());
				scl = glm::scale(objMat, trs.Scale());
				objMat = pos * rot * scl;
		


				std::cout << "// ----------------------------------------\n";

				std::cout << "cam trs matrix: \n["
					<< viewMtx[0][0] << ", "
					<< viewMtx[1][0] << ", "
					<< viewMtx[2][0] << ", "
					<< viewMtx[3][0] << "]\n["
					<< viewMtx[0][1] << ", "
					<< viewMtx[1][1] << ", "
					<< viewMtx[2][1] << ", "
					<< viewMtx[3][1] << "]\n["
					<< viewMtx[0][2] << ", "
					<< viewMtx[1][2] << ", "
					<< viewMtx[2][2] << ", "
					<< viewMtx[3][2] << "]\n["
					<< viewMtx[0][3] << ", "
					<< viewMtx[1][3] << ", "
					<< viewMtx[2][3] << ", "
					<< viewMtx[3][3] << ""
					<< "]\n";


				std::cout << "cam prj matrix: \n["
					<< projectionMtx[0][0] << ", "
					<< projectionMtx[1][0] << ", "
					<< projectionMtx[2][0] << ", "
					<< projectionMtx[3][0] << "]\n["
					<< projectionMtx[0][1] << ", "
					<< projectionMtx[1][1] << ", "
					<< projectionMtx[2][1] << ", "
					<< projectionMtx[3][1] << "]\n["
					<< projectionMtx[0][2] << ", "
					<< projectionMtx[1][2] << ", "
					<< projectionMtx[2][2] << ", "
					<< projectionMtx[3][2] << "]\n["
					<< projectionMtx[0][3] << ", "
					<< projectionMtx[1][3] << ", "
					<< projectionMtx[2][3] << ", "
					<< projectionMtx[3][3] << ""
					<< "]\n";


				std::cout << "matrix: \n["
					<< objMat[0][0] << ", "
					<< objMat[1][0] << ", "
					<< objMat[2][0] << ", "
					<< objMat[3][0] << "]\n["
					<< objMat[0][1] << ", "
					<< objMat[1][1] << ", "
					<< objMat[2][1] << ", "
					<< objMat[3][1] << "]\n["
					<< objMat[0][2] << ", "
					<< objMat[1][2] << ", "
					<< objMat[2][2] << ", "
					<< objMat[3][2] << "]\n["
					<< objMat[0][3] << ", "
					<< objMat[1][3] << ", "
					<< objMat[2][3] << ", "
					<< objMat[3][3] << ""
					<< "]\n";

				std::cout << "// ----------------------------------------\n";

				
				// need camera position.


				GLuint uniformLoc = glGetUniformLocation(program, "trs");
				glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(objMat));
				uniformLoc = glGetUniformLocation(program, "projection");
				glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(projectionMtx)); // constant
				uniformLoc = glGetUniformLocation(program, "view");
				glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(viewMtx));


			}
			


			// for now, position the frag shader


			

			glDrawElements(GL_TRIANGLES, mesh.IndexCount(), GL_UNSIGNED_INT, 0);


		}

	}
	glBindVertexArray(0);
}
