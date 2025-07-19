#include <arch/components/comp_meshrenderer.h>
#include <util/util_serialisation.h>


// - method function ------------------------

MeshRenderer::MeshRenderer() {
	SetComponentID(ComponentType::RENDER);
}

void MeshRenderer::Init() {
	LOG_INFO("Running Init");
	m_mesh.Init();
}

void MeshRenderer::End() {
	//
}

int MeshRenderer::VAO() const {
	return m_mesh.VAO();
}

int MeshRenderer::VBO() const {
	return 0;
}

void MeshRenderer::Render(
	const glm::vec4& _objectMatrix, 
	const glm::vec4& _projectionMatrix, 
	const glm::vec4& _cameraMatrix
) {
	
}

Material& MeshRenderer::GetDefaultMaterial() {
	static Material m_defaultMaterial{};
	static bool defaultMatInit	{ false };
	if (!defaultMatInit) {
		m_defaultMaterial.Init();



		std::string vertexShaderSource = FileReading::GetRawTextFromFile("./Assets/Shaders/vtx_vertex.vert");
		std::string fragmentShaderSource = FileReading::GetRawTextFromFile("./Assets/Shaders/frag_flatColor.frag");


		// - create a basic shader --------------------------------
		ShaderProgram dShader{};
		GLuint vtxShaderId  = ShaderProgram::LoadShader(vertexShaderSource.c_str(), ShaderProgram::VERTEX);
		GLuint fragShaderId = ShaderProgram::LoadShader(fragmentShaderSource.c_str(), ShaderProgram::FRAG);
		
		std::vector<GLuint> shaderList		{ vtxShaderId, fragShaderId };
		GLuint prg							{ ShaderProgram::LinkShaders(shaderList) };
		

		if (prg) dShader.SetShaderID(prg);

		// - move shader program to default material --------------
		m_defaultMaterial.SetShaderProgram(std::make_shared<ShaderProgram>(dShader));
		

		// - init is true -----------------------------------------
		defaultMatInit = true;
	}
	return m_defaultMaterial;
}





