#pragma once
#include <pch.h>
#include <arch/resources/res_material.h>
#include <arch/resources/res_texture/res_texture2d.h>




class LambertMaterial : public Material {

public:

	Materials::ShadingModel GetShadingModel() const override;

	// - getters -------------------------------------
	const glm::vec4& Color() const;
	void Color(const glm::vec4& _newColor);
	void Color(unsigned _newColor);

	void AlbedoTexture(const Texture2DHandle& _textureHandle);
	const Texture2DHandle& AlbedoTexture() const;

	void UsesColor(bool _usesTexture);
	bool UsesColor() const;

	//void ApplyUniforms() override;

	GLuint GetColorTextureID() const;
	// need a resource manager for textures

protected:
	void InitInternal() override;
	void ResolveUniformValues() override;
private:
	void SetupTextures();

private:

	// - color ---------------------------------------
	glm::vec4 m_color			{ 1, 1, 1, 1 };
	bool m_usesColor			{ true };

	GLuint m_reservedImageTexId	{}; // texture identifier, storage of image texture.
	
	Texture2DHandle m_textureColor;	// starts with a useless one for correctness
	Texture2DHandle m_albedoColor;	// starts invalid. assign with handle.


	GLuint m_reservedNmlTexId	{}; // texture identifier, storage of normal texture.



	INSPECTABLE_DECLAREPROPS(LambertMaterial);
};

struct LambertMaterialProps : public MaterialProps {
	LambertMaterialProps() : MaterialProps() {};
};


using LambertMaterialHandle = ResourceHandleDerivedT<Material, LambertMaterial>;