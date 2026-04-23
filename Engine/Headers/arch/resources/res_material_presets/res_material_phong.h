#pragma once
#include <pch.h>
#include <arch/resources/res_material.h>


#include <optional>


class PhongMaterial : public Material {

public:

	void Init() override;


	// - getters -------------------------------------
	
	// color
	const glm::vec4& GetColor() const;
	void SetColor(glm::vec4 _newColor);
	void SetColor(unsigned _newColor);
	const GLuint& GetColorImageTexture() const;
	void SetColorImageTexture(GLuint _textureId);

	void SetUsesColorValue(bool _usesColorValue);
	bool UsesColorValue() const;

	// specular
	const glm::vec4& GetSpecular() const;
	void SetSpecular(const glm::vec4& _newValue);
	void SetSpecular(unsigned _newColor);
	const GLuint& GetSpecularImageTexture() const;
	void SetSpecularImageTexture(const GLuint&) ;

	void SetUsesSpecularValue(bool _usesSpecularValue);
	bool UsesSpecularValue() const;


	// gloss
	const float& GetGloss() const;
	void SetGloss(float _value);
	const GLuint& GetGlossImageTexture() const;
	void SetGlossImageTexture(const GLuint& _texture);


	const GLuint& GetColorTextureID() const;
	const GLuint& GetSpecularTextureID() const;
	const GLuint& GetGlossTextureID() const;



	// need a better name!.
	void ApplyUniforms() const override;

	// need a resource manager for textures
private:
	void UpdateTextureID();
private:

	// - color ---------------------------------------
	glm::vec4 m_color{};
	bool m_usesColorValue{ true };

	glm::vec4 m_specularCol{ 1.0f };
	bool m_usesSpecularValue{ true };

	float m_glossVal{ 1.0f };
	bool m_usesGlossValue{ true };

	GLuint m_reservedColorTexId{};
	GLuint m_reservedColorImageTexId{};

	GLuint m_reservedSpecularTexId{};
	GLuint m_reservedSpecularImageTexId{};

	

	GLuint m_reservedGlossTexId{};
	GLuint m_reservedGlossImageTexId{};

};

