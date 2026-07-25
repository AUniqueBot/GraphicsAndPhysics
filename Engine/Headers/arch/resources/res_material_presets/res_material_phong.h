#pragma once
#include <pch.h>
#include <arch/resources/res_material.h>
#include <arch/resources/res_texture/res_texture2d.h>


#include <optional>


class PhongMaterial : public Material {

public:

	void Init() override;
	Materials::ShadingModel GetShadingModel() const override;

	// - getters -------------------------------------
	
	// color
	const glm::vec4& Color() const;
	void Color(const glm::vec4& _newColor);
	void Color(unsigned _newColor);
	const GLuint& GetColorImageTexture() const;
	void SetColorImageTexture(GLuint _textureId);

	void SetUsesColorValue(bool _usesColorValue);
	bool UsesColorValue() const;

	// specular
	const glm::vec4& Specular() const;
	void Specular(const glm::vec4& _newValue);
	void Specular(unsigned _newColor);
	const GLuint& GetSpecularImageTexture() const;
	void SetSpecularImageTexture(const GLuint&) ;

	void SetUsesSpecularValue(bool _usesSpecularValue);
	bool UsesSpecularValue() const;
	 
	 
	// gloss
	const float& Gloss() const;
	void Gloss(float _value);
	const GLuint& GetGlossImageTexture() const; 
	void SetGlossImageTexture(const GLuint& _texture); 
	 

	const GLuint& GetColorTextureID() const;
	const GLuint& GetSpecularTextureID() const;
	const GLuint& GetGlossTextureID() const; 
	 


	// need a better name!.
	void ApplyUniforms() const override; 

	// need a resource manager for textures
private:
	void SetupTextures();  
private:
	 
	// - color ---------------------------------------
	glm::vec4 m_color					{ 0.5f, 0.75f, 1.0f, 1.0f };
	bool m_usesColorValue				{ true };

	glm::vec4 m_specularCol				{ 0.0f, 1.0f, 1.0f, 1.0f };
	bool m_usesSpecularValue			{ true };
	 
	float m_glossVal					{ 1.0f };
	bool m_usesGlossValue				{ true };
	 

	Texture2D m_textureColor			{ TextureIDInfo{} }; 
	GLuint m_reservedColorTexId{}; 
	GLuint m_reservedColorImageTexId{};

	Texture2D m_textureSpecular			{ TextureIDInfo{} };
	GLuint m_reservedSpecularTexId{};
	GLuint m_reservedSpecularImageTexId{};  

	 
	Texture2D m_textureGloss			{ TextureIDInfo{} }; 
	GLuint m_reservedGlossTexId{}; 
	GLuint m_reservedGlossImageTexId{};  
	 
	INSPECTABLE_DECLAREPROPS(PhongMaterial);

};  
