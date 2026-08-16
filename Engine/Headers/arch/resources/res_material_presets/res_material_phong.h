#pragma once
#include <pch.h>
#include <arch/resources/res_material.h>
#include <arch/resources/res_texture/res_texture2d.h>


#include <optional>


class PhongMaterial : public Material {

public:

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
	 

	GLuint GetColorTextureID() const;
	GLuint GetSpecularTextureID() const;
	GLuint GetGlossTextureID() const; 

	void SetUsesGlossValue(bool _usesGlossValue);
	bool UsesGlossValue() const;
	 
	void Exponent(int _expVal);
	const int& Exponent() const;



	// need a resource manager for textures
protected:

	void InitInternal() override; 
	void ResolveUniformValues() override;

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
	 
	int m_exponentVal					{ 32 };

	bool m_valuesDirty					{ true };

	Texture2D m_textureColor;
	GLuint m_reservedColorTexId{}; 
	GLuint m_reservedColorImageTexId{};

	Texture2D m_textureSpecular;
	GLuint m_reservedSpecularTexId{};
	GLuint m_reservedSpecularImageTexId{};  

	
	// exponent + something

	Texture2D m_textureGloss;
	GLuint m_reservedGlossTexId{}; 
	GLuint m_reservedGlossImageTexId{};  
	


	INSPECTABLE_DECLAREPROPS(PhongMaterial);

};  


struct PhongMaterialHandle : public MaterialHandle {
	
	PhongMaterialHandle(ResourceIdentifierArg _arg) : MaterialHandle(_arg) {}
	

	inline void Color(unsigned _color) {
		auto res = GetResource<PhongMaterial>();
		if (res) res->Color(_color);
	}
	inline void Color(glm::vec4 _color) {
		auto res = GetResource<PhongMaterial>();
		if (res) res->Color(_color);
	}
	inline void Specular(unsigned _spec) {
		auto res = GetResource<PhongMaterial>();
		if (res) res->Specular(_spec);
	}
	inline void Specular(glm::vec4 _spec) {
		auto res = GetResource<PhongMaterial>();
		if (res) res->Specular(_spec);
	}
	inline void Gloss(float _gloss) {
		auto res = GetResource<PhongMaterial>();
		if (res) res->Gloss(_gloss);
	}

	inline glm::vec4 Color() const {
		auto res = GetResource<PhongMaterial>();
		return res ? res->Color() : glm::vec4(0.f);
	}
	inline glm::vec4 Specular() const {
		auto res = GetResource<PhongMaterial>();
		return res ? res->Specular() : glm::vec4(0.f);
	}
	inline float Gloss() const {
		auto res = GetResource<PhongMaterial>();
		return res ? res->Gloss() : 0.f;
	}

};