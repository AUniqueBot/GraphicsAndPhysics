#include <arch/resources/res_shader_generation/res_shader_generator.h>
#include <arch/resources/res_material_uniform/res_material_uniformAliases.h>


void ShaderGenerator::SetVersion(unsigned _major, unsigned _minor) {
	m_majorVersion = _major;
	m_minorVersion = _minor;
}

std::string ShaderGenerator::GenerateVersionHeader() const {
	std::stringstream ss;
	// delimiter
	unsigned verNo = m_majorVersion * 100 + m_minorVersion * 10;
	ss << "#version " << verNo  << " core" << "\n";
	return ss.str();
}

std::string ShaderGenerator::GenerateWorldObjectAttributes(const VAOHandler& _vaoHandler) const {

	std::stringstream ss;

	// attributes here.
	const char* attrLayoutString = 
		"layout (location = 0) in vec3 a_position;\n\n"
		"layout (location = 1) in vec3 a_normal;\n"
		"layout (location = 2) in vec3 a_uv;\n\n"
		;
	

	// uniforms here
	ss << 
		"uniform mat4 " << U_OBJECT_MATRIX << ";\n" <<
		"uniform mat4 " << U_CAMERA_MATRIX << ";\n" <<
		"uniform mat4 " << U_PROJECTION_MATRIX << ";\n" <<
		"\n";

	// outputs here
	const char* outputString =
		"out vec3 frag_position;\n\n"
		"out vec3 frag_normal;\n"
		"out vec3 frag_uv;\n\n"
		;

	return ss.str();
}

std::string ShaderGenerator::GenerateScreenSpaceAttributes() const {

	std::stringstream ss;


	const char* attrLayoutString =
		"layout (location = 0) in vec3 a_position;\n\n"
		"layout (location = 1) in vec3 a_normal;\n"
		"layout (location = 2) in vec3 a_uv;\n\n"
		;

	
	
	const char* uniformString =
		"uniform mat4 U_OBJ_MATRIX;\n\n"
		"uniform mat4 U_CAMERA_MATRIX;\n"
		"uniform mat4 U_PROJECTION_MATRIX;\n\n"

		;
	const char* outputString =
		"out vec3 frag_position;\n\n"
		"out vec3 frag_normal;\n"
		"out vec3 frag_uv;\n\n"
		;

	

	return std::string();
}

