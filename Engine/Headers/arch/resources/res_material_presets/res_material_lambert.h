#pragma once
#include <pch.h>
#include <arch/resources/res_material.h>
#include <arch/resources/res_texture/res_texture2d.h>

class LambertMaterial : public Material {

public:

	void Init() override;
	Materials::ShadingModel GetShadingModel() const override;

	// - getters -------------------------------------
	const glm::vec4& Color() const;
	void Color(const glm::vec4& _newColor);
	void Color(unsigned _newColor);

	void UsesColor(bool _usesTexture);
	bool UsesColor() const;

	void ApplyUniforms() const override;

	const GLuint& GetColorTextureID() const;
	// need a resource manager for textures
private:
	void SetupTextures();

private:

	// - color ---------------------------------------
	glm::vec4 m_color			{ };
	bool m_usesColor			{ true };

	GLuint m_reservedImageTexId	{}; // texture identifier, storage of image texture.
	
	Texture2D m_textureColor	{ TextureIDInfo{} }; // starts with a useless one for correctness

	GLuint m_reservedNmlTexId	{}; // texture identifier, storage of normal texture.



	INSPECTABLE_DECLAREPROPS(LambertMaterial);
};

