#pragma once
#include <pch.h>
#include <arch/resources/res_material.h>


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


	inline void SetTextureID(const GLuint& texId) { m_texId = texId; };
	inline GLuint GetTextureID() const { return m_texId; }


	void ApplyUniforms() const override;

	// need a resource manager for textures
private:
	void UpdateTextureID();
private:

	// - color ---------------------------------------
	glm::vec4 m_color			{};
	bool m_usesColor			{ true };
	GLuint m_texId				{}; // texture identifier, sent via uniform

	GLuint m_reservedImageTexId	{}; // texture identifier, storage of image texture.
	GLuint m_reservedColorTexId {}; // texture identifier, storage of color texture.
	
	GLuint m_reservedNmlTexId	{}; // texture identifier, storage of normal texture.
	
	INSPECTABLE_DECLAREPROPS(LambertMaterial);
};

