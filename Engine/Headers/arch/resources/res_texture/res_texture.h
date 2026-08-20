#pragma once
#include <pch.h>
#include <arch/resources/res_resource.h>
#include <arch/resources/res_resourceHandle.h>

#include <arch/resources/res_texture/res_imagedata.h>
#include <arch/resources/res_texture/res_texture_properties.h>
#include <arch/resources/res_gpu_resources/res_gpu_resourceHandle.h>

namespace TextureConstants {
	inline constexpr const RES_ID C_INVALID_TEXTURE_ID = ResourceConstants::C_RES_INVALID_ID;
};

class GPU_Texture;

class TextureRes : public Resource<TextureRes> {
public:


public:
	// - generic --------------------------------------------------
	const glm::ivec3& GetDimensions() const;
	const TextureProperties::TextureType& GetTextureType() const;


	// functions here assume you can access the texture.
	// getters particularly do not have safety checks so write accordingly
	// - properties -----------------------------------------------
	void SetMinFilter(const TextureProperties::FilterBehaviour& _filterBehaviour);
	void SetMagFilter(const TextureProperties::FilterBehaviour& _filterBehaviour);
	TextureProperties::FilterBehaviour GetMinFilter() const;
	TextureProperties::FilterBehaviour GetMagFilter() const;

	void SetWrapU(const TextureProperties::WrapBehaviour& _wrapBehaviour);
	void SetWrapV(const TextureProperties::WrapBehaviour& _wrapBehaviour);

	void SetInternalFormat(const TextureProperties::TextureFormat& _format); 

	void SetMipmapCount(int _mipCount);
	int GetMipmapCount() const;
	void SetAutogenMips(bool _autogen);
	bool GetAutogenMips() const;


	// - direct access functions -----------------------------------
	void SetPixelColor(unsigned _col, int _x, int _y, int _z); // hexadecimal color
	void SetPixelColor(glm::vec4 _col, int _x, int _y, int _z);


	// - other -----------------------------------------------------
	GLuint GetTextureHandle() const;


	bool InfoDirty() const;

	// - data ------------------------------------------------------
	const std::vector<ImageData>& GetImageData() const;
	const TextureProperties::TextureProps& GetTextureProps() const;
	void SetTextureProps(const TextureProperties::TextureProps& _props);


	// - gpu links -------------------------------------------------
	void SetGPUResourceHandle(GPUResourceHandle _newHandle);
	bool HasValidGPUResourceHandle() const;

protected:
	// be careful when using this as this assumes it is valid.
	GPU_Texture& GetGPUTexture();
	const GPU_Texture& GetGPUTexture() const;

protected:
	void FlagInfoClean();

protected:
	friend class GPUResorceManager;
	glm::ivec3 m_dimensions;
	GPUResourceHandle m_gpuResHandle;
	TextureProperties::TextureType m_textureType		{}; // static and cannot be changed after creation; per type.
	
	// - props ----------------------------------------------
	TextureProperties::TextureProps m_textureProps;

	// - data -----------------------------------------------
	std::vector<ImageData> m_imageData;
protected:
	bool m_infoDirty { true };
	
};

// -----------------------------------------------------------------------

using TextureHandle = ResourceHandleT<TextureRes>;
