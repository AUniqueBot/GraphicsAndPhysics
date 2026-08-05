#pragma once

#include <pch.h>
#include <array>
#include <variant>
#include <arch/resources/res_shader.h>
#include <arch/resources/res_material_uniform/res_material_uniform.h>
#include <arch/resources/res_material_uniform/res_material_uniformAliases.h>
#include <arch/common/entityid.h>
#include <arch/systems/sys_render_modules/sys_render_shadowMap.h>
#include <arch/resources/res_texture/res_texture2d.h>

namespace Materials {
	enum class ShadingModel {
		LAMBERT,			// 
		PHONG,				//
		BLINN_PHONG,		//
		COOK_TORRENCE,
		GGX,
		BURLEY,				//
		PRINCIPLED,
		NONE,
		_COUNT,
		CUSTOM				// for custom materials
	};
}

struct MaterialFeatures {
	bool isPBR;
	bool isLit;
	bool hasSpecular;
};

using MaterialValue = std::variant<
	bool,
	GLint,
	GLuint,
	GLfloat,
	glm::vec2,
	glm::vec3,
	glm::vec4,
	glm::ivec2,
	glm::ivec3,
	glm::ivec4,
	glm::mat4
>;

template<typename T>
concept MaterialValueType =
	requires(MaterialValue v) {
	std::get<T>(v);
};



struct MaterialValueData {

	enum class ValueType {
		Bool,
		Int,
		Unsigned,
		Float,
		// there are ivec and uvec variants but for this case we're ignoring those.
		Vec2,
		Vec3,
		Vec4,
		IVec2,
		IVec3,
		IVec4,
		Mat4f,
		Texture,
		_COUNT,
		INVALID
	};

	ValueType m_type		{ ValueType::Bool };
	MaterialValue m_value;

	template<MaterialValueType T>
	T& GetValue() {
		return std::get<T>(m_value);
	}

	template<MaterialValueType T>
	const T& GetValue() const {
		return std::get<T>(m_value);
	}

	template<MaterialValueType T>
	T GetValueCopy() const {
		return std::get<T>(m_value);
	}

	template<MaterialValueType T>
	T* TryGetValue() {
		return std::get_if<T>(&m_value);
	}

	template<MaterialValueType T>
	const T* TryGetValue() const {
		return std::get_if<T>(&m_value);
	}

	template<MaterialValueType T>
	bool Is() const {
		return std::holds_alternative<T>(m_value);
	}

	template<MaterialValueType T>
	void SetValue(const T& value) {
		m_value = value;
	}
};

MaterialValueData::ValueType OpenGL_ShaderTypeToValueType(GLenum type);
MaterialValue GetDefaultValue(MaterialValueData::ValueType type);





struct MaterialProps {
	std::string m_shaderId {""};
	// I want the shader ID provided by the thing
	
	std::map<std::string, MaterialValueData> m_props;
};





class RenderSystem; // fwd decl. 
// - class -----------------------------
class Material : public Resource<Material> {
public:
	using SHADERTYPE = ShaderConstants::ShaderType; // alias the name

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
	std::string ResourceTypeName() override { return "Material"; }
public:

	void Init();

	void SetShaderProgram(GLuint _shaderProg);
	int GetShaderProgram() const;

	// - shader programs --------------------------------------
	virtual Materials::ShadingModel GetShadingModel() const;
	void UseMaterial();



	void ApplyShadowMap(
		const unsigned int& _dirShadowId,
		const unsigned int& _spotShadowId,
		const unsigned int& _pointShadowId
	) const;
	void ApplyUniforms();


	// - apply uniform functions -----------------------------------------
	void SetUniformMat4f(std::string _uniformName, glm::mat4 _value);
	void SetUniformVec3(std::string _uniformName, glm::vec3 _value);
	void SetUniformVec2(std::string _uniformName, glm::vec2 _value);
	void SetUniformIVec3(std::string _uniformName, glm::ivec3 _value);
	void SetUniformIVec2(std::string _uniformName, glm::ivec2 _value);
	void SetUniformInt(std::string _uniformName, GLint _value);
	void SetUniformUnsigned(std::string _uniformName, GLuint _value);
	void SetUniformFloat(std::string _uniformName, GLfloat _value);
	void SetUniformTextureUnit(std::string _uniformName, GLuint _textureHandle, unsigned _index);


	void SetUniformMat4f(GLint _uniformLocation, glm::mat4 _value) const;
	void SetUniformVec4(GLint _uniformLocation, glm::vec4 _value) const;
	void SetUniformVec3(GLint _uniformLocation, glm::vec3 _value) const;
	void SetUniformVec2(GLint _uniformLocation, glm::vec2 _value) const;
	void SetUniformIVec4(GLint _uniformLocation, glm::ivec4 _value) const;
	void SetUniformIVec3(GLint _uniformLocation, glm::ivec3 _value) const;
	void SetUniformIVec2(GLint _uniformLocation, glm::ivec2 _value) const;
	void SetUniformInt(GLint _uniformLocation, GLint _value) const;
	void SetUniformUnsigned(GLint _uniformLocation, GLuint _value) const;
	void SetUniformFloat(GLint _uniformLocation, GLfloat _value) const;
	void SetUniformTextureUnit(GLint _uniformLocation, GLuint _textureHandle, unsigned _index) const;


	// - config -----------------------------------------------
	bool IsInitialised() const;
	void SetInitialised(bool _setting);

	
	

protected:
	virtual void InitInternal();
	GLint GetUniformLocation(const std::string& _uniformName) const;

	void InitUniformLocations();
	void SetUniform(std::string _uniformName, UniformData _data) const;

	virtual void ResolveTextureValues();

	GLuint m_shader								{};
	bool m_initialized							{};

	std::map<std::string, GLint> m_uniformLocations;
	SparseSet<std::string, MaterialValueData> m_materialValues;

	bool m_textureReferenceDirty				{ true }; // starts as true to initialize the values first.
	bool m_valuesDirty							{ true };
};