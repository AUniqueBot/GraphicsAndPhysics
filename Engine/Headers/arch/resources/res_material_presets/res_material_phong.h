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

	GLuint m_reservedColorTexId{};		// texture identifier, storage of color texture.
	GLuint m_reservedColorImageTexId{}; // texture identifier, storage of image texture.

	GLuint m_reservedSpecularTexId{};		// texture identifier, storage of specular 1x1 texture.
	GLuint m_reservedSpecularImageTexId{};	// texture identifier, storage of specular map texture.

	GLuint m_reservedGlossTexId{};
	GLuint m_reservedGlossImageTexId{};

};

