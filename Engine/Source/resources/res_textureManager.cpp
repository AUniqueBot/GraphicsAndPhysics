#include <arch/resources/res_textureManager.h>
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>


namespace {
	// loading helpers


	size_t GetPixelCount(glm::ivec3 _dimensions) {
		return static_cast<size_t>(_dimensions.x) * _dimensions.y * _dimensions.z;
	}

	size_t GetByteSize(TextureProperties::ImageDataType _dataType) {
		using namespace TextureProperties;
		switch (_dataType) {
		case ImageDataType::UINT_8:   return 1;
		//case ImageDataType::UINT16:  return 2;
		case ImageDataType::FLOAT_32: return 4;
		}
		return 0;
	}

	size_t GetChannelCount(TextureProperties::ImageChannels _channels) {
		using namespace TextureProperties;
		switch (_channels) {
		case ImageChannels::Red: return 1;
		case ImageChannels::RG: return 2;
		case ImageChannels::RGB: return 3;
		case ImageChannels::RGBA: return 4;
		case ImageChannels::Depth: return 0;
		}
		return 0;
	}

	struct ImageData {
		std::vector<std::byte> m_data;
		glm::ivec3 m_dimensions;
		TextureProperties::ImageChannels m_channels;
		TextureProperties::ImageChannels m_originalChannels;
		TextureProperties::ImageDataType m_dataType;

		ImageData(
			const void* _src,
			glm::ivec3 _dims,
			TextureProperties::ImageChannels _ogChannels,
			TextureProperties::ImageDataType _dataType
		)
			: m_dimensions(_dims),
			m_originalChannels(_ogChannels),
			m_channels(TextureProperties::ImageChannels::RGBA),
			m_dataType(_dataType)
		{
			size_t size = GetPixelCount(_dims) * GetByteSize(m_dataType) * GetChannelCount(m_channels);
			m_data.resize(size);
			std::memcpy(m_data.data(), _src, size);
		}
		ImageData(const ImageData&) = default;
		ImageData& operator=(const ImageData&) = default;

		template <typename T>
		const T* GetData() const {
			return reinterpret_cast<const T*>(m_data.data());
		}

		size_t GetImagePixelCount() const {
			return GetPixelCount(m_dimensions);
		}
		size_t GetComponentCount() const {
			return GetImagePixelCount() * static_cast<size_t>(m_channels);
		}
	};
	


	static ImageData LoadImage(const char* _path) {
		glm::ivec3 dimensions{ 0, 0, 1 };
		int channelCount{};

		bool isHDR = stbi_is_hdr(_path);
		void* data = isHDR ? 
			reinterpret_cast<void*>(stbi_loadf(_path, &dimensions.x, &dimensions.y, &channelCount, 4)) : // hdr
			reinterpret_cast<void*>(stbi_load(_path, &dimensions.x, &dimensions.y, &channelCount, 4));   // sdr
		using namespace TextureProperties;
		
		ImageChannels ogChannels =
			channelCount == 1 ? ImageChannels::Red :
			channelCount == 2 ? ImageChannels::RG :
			channelCount == 3 ? ImageChannels::RGB :
			ImageChannels::RGBA;
		ImageData output(data, dimensions, ogChannels, isHDR ? ImageDataType::FLOAT_32 : ImageDataType::UINT_8);
		stbi_image_free(data);
		return output;
	}

	template <typename T>
	static std::vector<T> ConvertRGBAToOtherChannel(
		const T* _data,
		size_t _pixelCount,
		TextureProperties::ImageChannels _destinationChannel
	) {
		size_t pixelCount { _pixelCount };
		size_t newChannelSize { GetChannelCount(_destinationChannel) };
		size_t newSize = pixelCount * newChannelSize;
		std::vector<T> output(newSize);
		for (size_t i{}; i < pixelCount; ++i) {
			size_t oldStrideStart = i * 4;
			size_t newStrideStart = i * newChannelSize;

			output[newStrideStart] = _data[oldStrideStart];
			if (newChannelSize < 2) continue;
			output[newStrideStart + 1] = _data[oldStrideStart + 1];
			if (newChannelSize < 3) continue;
			output[newStrideStart + 2] = _data[oldStrideStart + 2];
			if (newChannelSize < 4) continue;
			output[newStrideStart + 3] = _data[oldStrideStart + 3];
		}
		return output;
	}


	template <typename Src, typename Dst>
	inline static std::vector<Dst> ConvertDataType(const Src* _data, size_t _componentCount) {
		std::vector<Dst> output(_data, _data + _componentCount);
		return output;
	}

	template <>
	inline static std::vector<float> ConvertDataType(const uint8_t* _data, size_t _componentCount) {
		std::vector<float> output(_componentCount);
		for (size_t i{}; i < _componentCount; ++i) {
			output[i] = _data[i] / static_cast<float>(0xff);
		}
		return output;
	}

	template <>
	inline static std::vector<uint8_t> ConvertDataType(const float* _data, size_t _componentCount) {
		std::vector<uint8_t> output(_componentCount);
		for (size_t i{}; i < _componentCount; ++i) {
			output[i] = static_cast<uint8_t>(_data[i] * 0xff);
		}
		return output;
	}

}






bool TextureManager::TextureExists(RES_ID _id) const {
	return m_resourceIdPool.contains(_id);
}

void TextureManager::Resolve(TextureGPU& _texture) {
	

}





Texture2D TextureManager::LoadTexture(const std::filesystem::path& _path) {
	LOG_INFO("Loading Texture from \""<< _path <<"\"");


	ImageData img = LoadImage(_path.string().c_str());
	int channelCount = static_cast<int>(img.m_channels);
	int width = img.m_dimensions.x, height = img.m_dimensions.y;

	LOG_INFO("Loaded Metadata: [Width: " << width << ", Height: " << height << ", Channels: " << channelCount << "]");
	img.m_channels;
	img.m_dataType;
	img.m_originalChannels;
	

	// init required objects 
	using namespace TextureProperties;
	TextureProperties::TextureProps props;
	switch (channelCount) {
	case 1:
		props.m_internalImageFormat = TextureFormat::R8;
		props.m_pixelFormat = ImageChannels::Red;
		break;
	case 3:
		props.m_internalImageFormat = TextureFormat::RGB8;
		props.m_pixelFormat = ImageChannels::RGB;
		break;
	case 4:
		props.m_internalImageFormat = TextureFormat::RGBA8;
		props.m_pixelFormat = ImageChannels::RGBA;
		break;
	default:
		LOG_WARN("Unsupported number of channels: " << channelCount << ". Defaulting to RGBA8");
		props.m_internalImageFormat = TextureFormat::RGBA8;
		props.m_pixelFormat = ImageChannels::RGBA;
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
	// do conversion here.
	auto channelConverted = ConvertRGBAToOtherChannel(img.m_data.data(), img.GetImagePixelCount(), img.m_originalChannels);
	
	std::vector<uint8_t> tempByte8Buffer;
	std::vector<uint16_t> tempByte16Buffer;
	std::vector<float> tempFloatBuffer;


	void* upload = nullptr;
	if (img.m_dataType == tex.GetDataType()) upload = channelConverted.data();
	else {
		size_t componentCount = img.GetImagePixelCount() * GetChannelCount(img.m_originalChannels);

		switch (img.m_dataType) {
		case ImageDataType::UINT_8: {
			tempByte8Buffer = ConvertDataType<float, uint8_t>(
				reinterpret_cast<const float*>(channelConverted.data()), 
				img.GetComponentCount()
			);
			upload = tempByte8Buffer.data();
			break;
		}
		case ImageDataType::FLOAT_32: {
			tempFloatBuffer = ConvertDataType<uint8_t, float>(
				reinterpret_cast<const uint8_t*>(channelConverted.data()), 
				img.GetImagePixelCount()
			);
			upload = tempFloatBuffer.data();
			break;
		}
		}
	}
	uploadData.m_textureData = upload;
	uploadData.m_dimensions = { width, height, 1 };
	uploadData.m_mipLevel = 0;
	
	tex.Allocate();
	tex.Upload({ {uploadData} });// upload data here.

	// if successful add to storage
	std::shared_ptr<TextureRes> texHandle = std::make_shared<TextureRes>();
	ResourceIdentifier id = m_resourceManager.AddInternalResource(texHandle);
	m_textureGPUStorage.Add(std::move(tex), id.m_resourceId);
	m_resourceIdPool.insert(id.m_resourceId);
	Texture2D retVal{ id };
	return retVal;
}


// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
Texture2D TextureManager::Create2DTexture(int width, int height, TextureProperties::TextureProps _props) {
	TextureGPU tex{
		TextureProperties::TextureType::TEXTURE_2D,
		{ width, height, 1 },
		_props
	};

	tex.Create();
	tex.Allocate();
	std::shared_ptr<TextureRes> texHandle = std::make_shared<TextureRes>();
	ResourceIdentifier id = m_resourceManager.AddInternalResource(texHandle);
	m_textureGPUStorage.Add(std::move(tex), id.m_resourceId);
	m_resourceIdPool.insert(id.m_resourceId);
	LOG_INFO("Allocating 2D Texture of size: [" << width << ", " << height << "]");
	Texture2D retVal{ id };
	return retVal;
}

void TextureManager::Create3DTexture(int _width, int _height, int _depth, TextureProperties::TextureProps _props) {

	TextureGPU tex = { 
		TextureProperties::TextureType::TEXTURE_3D,
		{ _width, _height, _depth },
		_props
	};
	tex.Create();
	tex.Allocate();
	m_textureGPUStorage.Add(std::move(tex), C_INVALID_TEXTURE_ID);

	//return info;
}

Texture2DArray TextureManager::Create2DArrayTexture(int _width, int _height, int _layers, TextureProperties::TextureProps _props) {
	TextureGPU tex = TextureGPU(
		TextureProperties::TextureType::TEXTURE_2D_ARRAY,
		{ _width, _height, _layers },
		_props
	);

	tex.Create();
	tex.Allocate();
	std::shared_ptr<TextureRes> texHandle = std::make_shared<TextureRes>();
	ResourceIdentifier id = m_resourceManager.AddInternalResource(texHandle);
	LOG_INFO("Allocating 2D Texture array of size: [" << _width << ", " << _height << "] with " << _layers << " layers.");
	m_textureGPUStorage.Add(std::move(tex), id.m_resourceId);
	m_resourceIdPool.insert(id.m_resourceId);
	Texture2DArray retVal{id};
	return retVal;
}

void TextureManager::CreateCubemapTexture(int width, int height, TextureProperties::TextureProps _props) {
	

}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------

SparseSetView<TextureGPU> TextureManager::GetTexture(RES_ID _id) {
	return m_textureGPUStorage.At(_id);
}


void TextureManager::UpdateTextures() {
	for (TextureGPU& tex : m_textureGPUStorage.Data()) {
		tex.UpdateAllocation();
		tex.UpdateTextureProperties();
	}
}


