#pragma once
#include <pch.h>
#include <arch/resources/res_texture/res_texture.h>
#include <arch/resources/res_gpu_resources/res_gpu_resource.h>
#include <arch/resources/res_texture/res_texture_properties.h>

// Internal GPU class for texture
// handles raw allocation and upload logic
class GPU_Texture : public GPU_Resource {
public:
	GPU_Texture(
		TextureProperties::TextureType _type,
		glm::ivec3 _dims = glm::ivec3(1, 1, 1),
		TextureProperties::TextureProps _props = TextureProperties::TextureProps{}
	);
	~GPU_Texture();
	GPU_Texture(GPU_Texture&& _old) noexcept;
	GPU_Texture& operator=(GPU_Texture&& _old) noexcept;

	GPU_Texture(GPU_Texture& _old) = delete;
	GPU_Texture& operator=(GPU_Texture& _old) = delete;
public:

	void Load(const TextureRes& _tex);

	const GLuint& GetTextureHandle() const;
	const TextureProperties::TextureType& GetTextureType() const;


	// - tex properties ------------------
	const TextureProperties::TextureFormat& GetInternalImageFormat() const;
	void SetInternalImageFormat(const TextureProperties::TextureFormat& _format);
	const TextureProperties::ImageDataType& GetDataType() const;
	void SetDataType(const TextureProperties::ImageDataType& _pixelDataType);
	const TextureProperties::ImageChannels& GetChannels() const;
	void SetChannels(const TextureProperties::ImageChannels& _pixelFormat);


	// - sampling behaviours -------------
	void SetWrapBehaviourU(const TextureProperties::WrapBehaviour& _wrapBehaviour);
	const TextureProperties::WrapBehaviour& GetWrapBehaviourU() const;
	void SetWrapBehaviourV(const TextureProperties::WrapBehaviour& _wrapBehaviour);
	const TextureProperties::WrapBehaviour& GetWrapBehaviourV() const;

	void SetFilterBehaviourMin(const TextureProperties::FilterBehaviour& _filterBehaviour);
	const TextureProperties::FilterBehaviour& GetFilterBehaviourMin() const;
	void SetFilterBehaviourMag(const TextureProperties::FilterBehaviour& _filterBehaviour);
	const TextureProperties::FilterBehaviour& GetFilterBehaviourMag() const;

	// - dimension setters ---------------
	void SetDimensions(glm::ivec3 _dims);
	const glm::ivec3& GetDimensions() const;
	const int& GetX() const;
	void SetX(const int& _val);

	const int& GetY() const;
	void SetY(const int& _val);

	const int& GetZ() const;
	void SetZ(const int& _val);


	// - pixel setter -------------------
	void SetPixelColor(glm::u8vec1 _col, glm::ivec3 _pixelPos);	// r
	void SetPixelColor(glm::u8vec2 _col, glm::ivec3 _pixelPos);	// rg
	void SetPixelColor(glm::u8vec3 _col, glm::ivec3 _pixelPos);	// rgb
	void SetPixelColor(glm::u8vec4 _col, glm::ivec3 _pixelPos);	// rgba 



public:
	void Create() override;
	void Destroy() override;
	void Allocate();
	void Upload(TextureProperties::TextureUploadData _imageData = std::nullopt) const;
	bool NeedUpdate() const;
	bool NeedAllocate() const;
public:


private:
	void UploadTexture2DData(TextureProperties::TextureUploadData _imageData = std::nullopt) const;
	void UploadCubemapData(TextureProperties::TextureUploadData _imageData = std::nullopt) const;
	void Upload3DTextureData(TextureProperties::TextureUploadData _imageData = std::nullopt) const;
	void UploadCubemapArrayData(TextureProperties::TextureUploadData _imageData = std::nullopt) const;
	void UploadTexture2DArrayData(TextureProperties::TextureUploadData _imageData = std::nullopt) const;


protected:
	friend class TextureManager;
	void UpdateTextureProperties();
	void UpdateAllocation();
protected:

	// 
	bool m_reallocateDirty{};
	bool m_samplingDirty{};
	bool m_allocated{};
	bool m_uploaded{};
	bool m_uploadNeedUpdate{};

	// 
	TextureProperties::TextureType m_textureType;

	glm::ivec3 m_dimensions;
	TextureProperties::TextureProps m_textureProperties;
};