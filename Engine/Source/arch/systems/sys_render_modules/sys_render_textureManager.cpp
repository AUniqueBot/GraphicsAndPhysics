#include <arch/systems/sys_render_modules/sys_render_textureManager.h>
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>


TextureIDInfo TextureManager::GenerateTextureIDInfo() {
	TextureID nextId;
	if (!m_freeIds.empty()) {
		nextId = m_freeIds.front();
		m_freeIds.pop();
	}
	else {
		nextId = m_nextID++;
	}
	return TextureIDInfo(nextId, this);
}

void TextureManager::ReclaimTextureIDInfo(TextureIDInfo _id) {
	m_storage.Remove(_id.GetTextureID());
	m_freeIds.push(_id.GetTextureID());
}

bool TextureManager::TextureExists(TextureID _id) const {
	return static_cast<bool>(m_storage.At(_id));
}

void TextureManager::Resolve(TextureGPU& _texture) {
	



}

Texture2D TextureManager::LoadTexture(const std::filesystem::path& _path) {
	LOG_INFO("Loading Texture from \""<< _path <<"\"");
	int width { 0 }, height { 0 };
	// number of channels indicates how to allocate texture.
	int channels { 0 };
	void* imgData = stbi_load(_path.string().c_str(), &width, &height, &channels, 0);
	LOG_INFO("Loaded Metadata: [Width: " << width << ", Height: " << height << ", Channels: " << channels << "]");

	// init required objects 
	TextureIDInfo info = GenerateTextureIDInfo();
	using namespace TextureProperties;
	TextureProperties::TextureProps props;
	switch (channels) {
	case 1:
		props.m_internalImageFormat = InternalImageFormat::R8;
		props.m_pixelFormat = PixelFormat::RED;
		break;
	case 3:
		props.m_internalImageFormat = InternalImageFormat::RGB8;
		props.m_pixelFormat = PixelFormat::RGB;
		break;
	case 4:
		props.m_internalImageFormat = InternalImageFormat::RGBA8;
		props.m_pixelFormat = PixelFormat::RGBA;
		break;
	default:
		LOG_WARN("Unsupported number of channels: " << channels << ". Defaulting to RGBA8");
		props.m_internalImageFormat = InternalImageFormat::RGBA8;
		props.m_pixelFormat = PixelFormat::RGBA;
		break;
	}

	TextureGPU tex = { 
		TextureProperties::TextureType::TEXTURE_2D,
		{width, height, 1},
		props
	};
	tex.Create();

		

	// upload data
	TextureProperties::ImageUploadData uploadData;
	uploadData.m_textureData = imgData;
	uploadData.m_dimensions = { width, height, 1 };
	uploadData.m_mipLevel = 0;
	
	tex.Allocate();
	tex.Upload({ {uploadData} });// upload data here.

	// if successful add to storage
	m_storage.Add(std::move(tex), info.GetTextureID());
	Texture2D texHandle = { info };
	return texHandle;
}


// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
Texture2D TextureManager::Create2DTexture(int width, int height, TextureProperties::TextureProps _props) {
	
	
	TextureIDInfo info = GenerateTextureIDInfo();
	TextureGPU tex = {
		TextureProperties::TextureType::TEXTURE_2D,
		{ width, height, 1 },
		_props
		
	};
	m_storage.Add(std::move(tex), info.GetTextureID());
	tex.Create();
	tex.Allocate();
	LOG_INFO("Allocating 2D Texture of size: [" << width << ", " << height << "]");
	Texture2D texHandle = { info };
	return texHandle;
}

TextureIDInfo TextureManager::Create3DTexture(int width, int height, int depth, TextureProperties::TextureProps _props) {
	TextureIDInfo info = GenerateTextureIDInfo();
	TextureGPU tex = { 
		TextureProperties::TextureType::TEXTURE_3D,
		{ width, height, depth },
		_props
	};
	m_storage.Add(std::move(tex),info.GetTextureID());
	return info;
}

TextureIDInfo TextureManager::Create2DArrayTexture(int width, int height, int layers, TextureProperties::TextureProps _props) {
	TextureIDInfo info = GenerateTextureIDInfo();
	m_storage.Add(
		TextureGPU(
			TextureProperties::TextureType::TEXTURE_2D_ARRAY,
			{ width, height, layers },
			_props
		),
		info.GetTextureID()
	);

	return info;
}

TextureIDInfo TextureManager::CreateCubemapTexture(int width, int height, TextureProperties::TextureProps _props) {
	TextureIDInfo info = GenerateTextureIDInfo();
	m_storage.Add(
		TextureGPU(
			TextureProperties::TextureType::CUBEMAP,
			{ width, height, 6 },
			_props
		),
		info.GetTextureID()
	);

	return info;
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------

SparseSetView<TextureGPU> TextureManager::GetTexture(TextureID _id) {
	return m_storage.At(_id);
}

void TextureManager::DeleteTexture(TextureIDInfo _id) {
	SparseSetView<TextureGPU> handle = GetTexture(_id.GetTextureID());
	if (!handle) {
		LOG_WARN("Attempted to delete non existent texture. Ignoring.");
		return;
	}
	TextureGPU& tex = *handle;
	// reclaim ID.
	m_freeIds.push(_id.GetTextureID());
	tex.Destroy(); // destroy if it wasn't already.
}


