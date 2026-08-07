#pragma once
#include <pch.h>
#include <arch/resources/res_resource.h>
#include <arch/resources/res_resourceManager.h>
#include <arch/resources/res_texture/res_texturegpu.h>
#include <arch/resources/res_texture/res_texture_properties.h>


class Texture;
inline const RES_ID C_INVALID_TEXTURE_ID = BaseResource::C_RES_ID_INVALID;



class TextureRes : public Resource<TextureRes> {
	// a simple handle to the thing.
public:		
	std::string ResourceTypeName() override { return "Texture"; };
	TextureGPU& GetTextureGPU();
	const TextureGPU& GetTextureGPU() const;
private:


};


struct TextureHandle : public ResourceHandle {
	inline TextureHandle(ResourceIdentifierArg _resIdArg) : ResourceHandle(_resIdArg) {}
	inline std::shared_ptr<TextureRes> Get() {
		return GetResource<TextureRes>();
	}
	inline std::shared_ptr<const TextureRes> Get() const {
		return GetResource<TextureRes>();
	}
};


// handles
// texture manager can only construct directly. otherwise, you can only construct from a copy.
class Texture : public TextureHandle {
public:
	Texture(ResourceIdentifierArg _resIdArg); // get ResourceIdentifier instead.
	Texture(const Texture& _other) = default;
	Texture& operator=(const Texture& _other) = default;
public:
	// functions here assume you can access the texture.
	// getters particularly do not have safety checks so write accordingly
	void SetMinFilter(const TextureProperties::FilterBehaviour& _filterBehaviour);
	void SetMagFilter(const TextureProperties::FilterBehaviour& _filterBehaviour);
	TextureProperties::FilterBehaviour GetMinFilter() const;
	TextureProperties::FilterBehaviour GetMagFilter() const;


	void SetWrapU(const TextureProperties::WrapBehaviour& _wrapBehaviour);
	void SetWrapV(const TextureProperties::WrapBehaviour& _wrapBehaviour);

	void SetInternalFormat(const TextureProperties::TextureFormat& _format); 
	//void SetPixelDataType(const TextureProperties::ImageDataType& _pixelType);
	//void SetPixelFormat(const TextureProperties::ImageChannels& _pixelFormat);

	void SetPixelColor(unsigned _col, int _x, int _y, int _z); // hexadecimal color
	void SetPixelColor(glm::vec4 _col, int _x, int _y, int _z);

	GLuint GetTextureHandle() const;

	bool TextureIsValid() const;

protected:
	// be careful when using this as this assumes it is valid.
	TextureGPU& GetTextureGPU();
	const TextureGPU& GetTextureGPU() const;

protected:
	TextureProperties::TextureType m_textureType{}; // static and cannot be changed after creation; per type.
};
