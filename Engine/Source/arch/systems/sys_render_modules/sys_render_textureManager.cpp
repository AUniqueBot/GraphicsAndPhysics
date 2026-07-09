#include <arch/systems/sys_render_modules/sys_render_textureManager.h>
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>



namespace {
	// loading helpers
	enum class ImageChannels {
		Red = 1,
		RG = 2,
		RGB = 3,
		RGBA = 4
	};
	enum class ImageDataType {
		UINT8,
		UINT16,
		FLOAT32,
	};

	size_t GetPixelCount(glm::ivec3 _dimensions) {
		return static_cast<size_t>(_dimensions.x) * _dimensions.y * _dimensions.z;
	}

	size_t GetByteSize(ImageDataType _dataType) {
		switch (_dataType) {
		case ImageDataType::UINT8:   return 1;
		case ImageDataType::UINT16:  return 2;
		case ImageDataType::FLOAT32: return 4;
		}
		return 0;
	}

	struct ImageData {
		std::vector<float> m_data;
		glm::ivec3 m_dimensions;
		ImageChannels m_channels;
		ImageChannels m_originalChannels;
		ImageDataType m_dataType;

		ImageData(
			const float* _src,
			glm::ivec3 _dims,
			ImageChannels _ogChannels
		)
			: m_dimensions(_dims),
			m_originalChannels(_ogChannels),
			m_channels(ImageChannels::RGBA),
			m_dataType(ImageDataType::FLOAT32)
		{
			size_t size = GetPixelCount(_dims) * GetByteSize(m_dataType) * static_cast<int>(m_channels);
			m_data.resize(size);
			std::memcpy(m_data.data(), _src, size);
		}
		ImageData(const ImageData&) = default;
		ImageData& operator=(const ImageData&) = default;

	};
	


	static ImageData LoadImage(const char* _path) {
		glm::ivec3 dimensions{ 0, 0, 1 };
		int channelCount{};
		const float* data = stbi_loadf(_path, &dimensions.x, &dimensions.y, &channelCount, 4);
		ImageChannels ogChannels =
			channelCount == 1 ? ImageChannels::Red :
			channelCount == 2 ? ImageChannels::RG :
			channelCount == 3 ? ImageChannels::RGB :
			ImageChannels::RGBA;
		ImageData output(data, dimensions, ogChannels);
		stbi_image_free((void*)data);
		return output;
	}


	static std::vector<float> ConvertRGBAToOtherChannel(
		const std::vector<float>& _data, 
		ImageChannels _destinationChannel
	) {
		
	}



		// convert to other type.
	


	namespace OpenGL {
		// specifically for images a depth component image is not possible so it's ignored.
		static GLenum ResolveChannels(ImageChannels _channels) {
			return
				_channels == ImageChannels::Red ? GL_RED :
				_channels == ImageChannels::RG ? GL_RG :
				_channels == ImageChannels::RGB ? GL_RGB :
				GL_RGBA;
		}

		static GLenum ResolveDataType(ImageDataType _dataType) {
			return
				_dataType == ImageDataType::UINT8 ? GL_UNSIGNED_BYTE :
				_dataType == ImageDataType::UINT16 ? GL_UNSIGNED_SHORT :
				GL_FLOAT;
		}
	}




}


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
	ImageData img = LoadImage(_path.string().c_str());
	int channelCount = static_cast<int>(img.m_channels);
	int width = img.m_dimensions.x, height = img.m_dimensions.y;

	LOG_INFO("Loaded Metadata: [Width: " << width << ", Height: " << height << ", Channels: " << channelCount << "]");

	// init required objects 
	TextureIDInfo info = GenerateTextureIDInfo();
	using namespace TextureProperties;
	TextureProperties::TextureProps props;
	switch (channelCount) {
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
		LOG_WARN("Unsupported number of channels: " << channelCount << ". Defaulting to RGBA8");
		props.m_internalImageFormat = InternalImageFormat::RGBA8;
		props.m_pixelFormat = PixelFormat::RGBA;
		break;
	}

	TextureGPU tex = { 
		TextureProperties::TextureType::TEXTURE_2D,
		{img.m_dimensions.x, img.m_dimensions.y, 1},
		props
	};
	tex.Create();

		

	// upload data
	TextureProperties::ImageUploadData uploadData;
	uploadData.m_textureData = img.m_data.data();
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


