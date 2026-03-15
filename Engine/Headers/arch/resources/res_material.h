#pragma once

#include <pch.h>
#include <array>
#include <variant>
#include <arch/resources/res_shader.h>
#include <arch/resources/res_material_uniform/res_material_uniform.h>
#include <arch/resources/res_material_uniform/res_material_uniformAliases.h>
#include <arch/common/entityid.h>
#include <arch/systems/sys_render_modules/sys_render_shadowMap.h>



// - class -----------------------------
class Material {
public:
	using SHADERTYPE = ShaderProgram::SHADERTYPE; // alias the name

public:
	enum IMAGE_CLAMP_BEHAVIOUR {
		REPEAT,
		MIRROR_REPEAT,
		TO_EDGE,
		TO_BORDER
	};

	enum FILTER_TYPE {
		LINEAR,
		NEAREST
	};



public:

	virtual void Init();

	void SetShaderProgram(std::shared_ptr<ShaderProgram> _shaderProg);

	int GetShader() const;

	void ApplyShadowMap(const ShadowMap& _shadowMap) const;
	virtual void ApplyUniforms() const;


	// - apply uniform functions -----------------------------------------
	void SetUniformMatrix(std::string _uniformName, glm::mat4 _value);
	void SetUniformVec3(std::string _uniformName, glm::vec3 _value);
	void SetUniformVec2(std::string _uniformName, glm::vec2 _value);
	void SetUniformIVec3(std::string _uniformName, glm::ivec3 _value);
	void SetUniformIVec2(std::string _uniformName, glm::ivec2 _value);
	void SetUniformInt(std::string _uniformName, GLint _value);
	void SetUniformUnsigned(std::string _uniformName, GLuint _value);
	void SetUniformFloat(std::string _uniformName, GLfloat _value);
	


	// - material helpers -------------------------------------
	static unsigned LoadImage(std::string path, bool _hasAlpha, IMAGE_CLAMP_BEHAVIOUR _horizontal, IMAGE_CLAMP_BEHAVIOUR _vertical, FILTER_TYPE _fType);

	// - tex generator ----------------------------------------
	static GLuint GenerateEmptyColorTexture();									// this function generates a 1x1 texture. No data is filled.
	static void DeleteTexture(const GLuint& _id);								// this function deletes any texture.
	static void UpdateColorTexture(const GLuint& _id, const glm::vec4& _col);	// this function changes a 1x1 texture color.
	static std::array<GLubyte, 4> ColorToBytes(const glm::vec4& col);

	
protected:
	GLint GetUniformLocation(const std::string& _uniformName) const;

	void InitUniformLocations();
	void SetUniform(std::string _uniformName, UniformData _data) const;

	std::shared_ptr<ShaderProgram> m_shader;


	std::map<std::string, UniformData> m_uniformData;
	std::map<std::string, GLint> m_uniformLocations;

};