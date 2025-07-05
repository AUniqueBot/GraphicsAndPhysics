#include "pch.h"
#include <arch/resources/res_material.h>


static std::string GetRawText(std::string _pathToFile) {
	std::stringstream toRet;
	std::ifstream ifs{ _pathToFile };
	if (!ifs.good()) return std::string{};
	toRet << ifs.rdbuf();
	return toRet.str();
}

unsigned prg = 0;
unsigned flatPrg = 0;
unsigned matPrg = 0;

// example function, don't delete.
void LoadShaders() {


	// 
	std::string srcS{};
	const char* src{};
	int glStatus{};


	int vtxShader = 0;
	int fragShader = 0;
	int flatShader = 0;
	int texShader = 0;


	// vertex shader
	vtxShader = glCreateShader(GL_VERTEX_SHADER);	// assign this id to a vertex shader
	srcS = GetRawText("Assets/Shaders/vertexShader.vert");
	src = srcS.c_str();
	glShaderSource(vtxShader, 1, &src, NULL);
	glCompileShader(vtxShader);
	glGetShaderiv(vtxShader, GL_COMPILE_STATUS, &glStatus);

	if (glStatus == GL_FALSE) {
		char log[512];
		glGetShaderInfoLog(vtxShader, 512, NULL, log);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << log << std::endl;
	}

	// fragment shader
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	srcS = GetRawText("Assets/Shaders/phongShader.frag");
	src = srcS.c_str();
	glShaderSource(fragShader, 1, &src, NULL); // load shader source
	glCompileShader(fragShader); // compile the shaders.
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &glStatus);

	if (glStatus == GL_FALSE) {
		char log[512];
		glGetShaderInfoLog(fragShader, 512, NULL, log);
		std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << log << std::endl;
	}


	flatShader = glCreateShader(GL_FRAGMENT_SHADER);
	srcS = GetRawText("Assets/Shaders/flatShader.frag");
	src = srcS.c_str();
	glShaderSource(flatShader, 1, &src, NULL); // load shader source
	glCompileShader(flatShader); // compile the shaders.
	glGetShaderiv(flatShader, GL_COMPILE_STATUS, &glStatus);

	if (glStatus == GL_FALSE) {
		char log[512];
		glGetShaderInfoLog(flatShader, 512, NULL, log);

		std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << log << std::endl;
	}


	texShader = glCreateShader(GL_FRAGMENT_SHADER);
	srcS = GetRawText("Assets/Shaders/texturedFragShader.frag");
	src = srcS.c_str();
	glShaderSource(texShader, 1, &src, NULL); // load shader source
	glCompileShader(texShader); // compile the shaders.
	glGetShaderiv(texShader, GL_COMPILE_STATUS, &glStatus);
	if (glStatus == GL_FALSE) {
		char log[512];
		glGetShaderInfoLog(texShader, 512, NULL, log);
		std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << log << std::endl;
	}






	// creation and compiling the shader
	// can be done in update.
	prg = glCreateProgram();
	flatPrg = glCreateProgram();
	matPrg = glCreateProgram();

	glAttachShader(prg, fragShader);
	glAttachShader(prg, vtxShader);
	glLinkProgram(prg);

	glAttachShader(flatPrg, flatShader);
	glAttachShader(flatPrg, vtxShader);
	glLinkProgram(flatPrg);

	glAttachShader(matPrg, texShader);
	glAttachShader(matPrg, vtxShader);
	glLinkProgram(matPrg);



	glGetShaderiv(prg, GL_LINK_STATUS, &glStatus);
	if (glStatus == GL_FALSE) {
		char log[512];
		glGetShaderInfoLog(prg, 512, NULL, log);

		std::cout << "ERROR::SHADER::LINK::COMPILATION_FAILED\n" << log << std::endl;
	}

	glGetShaderiv(flatPrg, GL_LINK_STATUS, &glStatus);
	if (glStatus == GL_FALSE) {
		char log[512];
		glGetShaderInfoLog(flatPrg, 512, NULL, log);

		std::cout << "ERROR::SHADER::LINK::COMPILATION_FAILED\n" << log << std::endl;
	}

	glGetShaderiv(matPrg, GL_LINK_STATUS, &glStatus);
	if (glStatus == GL_FALSE) {
		char log[512];
		glGetShaderInfoLog(matPrg, 512, NULL, log);

		std::cout << "ERROR::SHADER::LINK::COMPILATION_FAILED\n" << log << std::endl;
	}


	// deletion can be done after compile.
	glDeleteShader(vtxShader);
	glDeleteShader(fragShader);
	glDeleteShader(flatShader);
	glDeleteShader(texShader);


}




void Material::Init() {
    
}

void Material::SetShaderProgram(std::shared_ptr<ShaderProgram> shaderProg){
	m_shader = std::move(shaderProg);
}

int Material::GetShader() const {
    return (m_shader.get() == nullptr) ? 0 : m_shader.get()->ShaderID();
}

