
#include <pch.h>
#include <arch/components/comp_meshrenderer.h>


// - statics --------------------------------
// general shaders.
const char* defaultVtxShader{
	"#version 330 core\n"
	"// a basic vertex shader that only takes in a vertex's positions.\n"
	"// this describes how the vertex data is like -> [VEC3 : Pos] [VEC2 : UV]\n"
	"layout(location = 0) in vec3 aPos;\n"
	"out vec3 fragPos;	// out position for every pixel\n"
	"uniform mat4 trs;			// object transform matrix\n"
	"uniform mat4 view;			// camera's view matrix (pos + rot, inverse)\n"
	"uniform mat4 projection;	// camera's view frustum\n"
	"void main() {\n"
	"	gl_Position = projection * view * trs * vec4(aPos, 1.0f);\n"
	"	fragPos = vec3(trs * vec4(aPos, 1.0));\n"
	"}\n"
};

const char* defaultFragShader{
	"#version 330 core\n"
	"in vec3 fragPos;\n"
	"out vec4 outCol;\n"
	"void main() {\n"
	"    // Map world position from [-1,1] to [0,1] for colour display\n"
	"    vec3 colour = fragPos * 0.5 + 0.5;\n"
	"    outCol = vec4(colour, 1.0);\n"
	"}\n"
};

// - method function ------------------------

MeshRenderer::MeshRenderer() {
	SetComponentID(ComponentType::RENDER);
}

void MeshRenderer::Init() {
	std::cout << "[MeshRenderer]::Init()" << std::endl;
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

Material& MeshRenderer::GetDefaultMaterial() {
	static Material m_defaultMaterial{};
	static bool defaultMatInit	{ false };
	if (!defaultMatInit) {
		m_defaultMaterial.Init();


		// - create a basic shader --------------------------------
		ShaderProgram dShader{};
		GLuint vtxShaderId  = ShaderProgram::LoadShader(defaultVtxShader,  ShaderProgram::VERTEX);
		GLuint fragShaderId = ShaderProgram::LoadShader(defaultFragShader, ShaderProgram::FRAG);
		
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





