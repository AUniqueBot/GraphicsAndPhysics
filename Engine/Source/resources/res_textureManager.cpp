#include <arch/resources/res_textureManager.h>
#include <arch/resources/res_gpu_resources/res_gpu_resourceHandle.h>



Texture2D TextureManager::LoadTexture(const std::filesystem::path& _path) {
	LOG_INFO("Loading Texture from \""<< _path <<"\"");


	ImageData img = ImageData::LoadImage(_path);
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

	GPU_Texture tex = { 
		TextureProperties::TextureType::TEXTURE_2D,
		{img.m_dimensions.x, img.m_dimensions.y, 1},
		props
	};
	tex.Create();

		

	// upload data
	TextureProperties::ImageUploadData uploadData;
	// do conversion here.
	auto channelConverted = ImageDataHelpers::ConvertRGBAToOtherChannel(img.m_data.data(), img.GetImagePixelCount(), img.m_originalChannels);
	
	std::vector<uint8_t> tempByte8Buffer;
	std::vector<uint16_t> tempByte16Buffer;
	std::vector<float> tempFloatBuffer;


	void* upload = nullptr;
	if (img.m_dataType == tex.GetDataType()) upload = channelConverted.data();
	else {
		size_t componentCount = img.GetImagePixelCount() * ImageDataHelpers::GetChannelCount(img.m_originalChannels);

		switch (img.m_dataType) {
		case ImageDataType::UINT_8: {
			tempByte8Buffer = ImageDataHelpers::ConvertDataType<float, uint8_t>(
				reinterpret_cast<const float*>(channelConverted.data()), 
				img.GetComponentCount()
			);
			upload = tempByte8Buffer.data();
			break;
		}
		case ImageDataType::FLOAT_32: {
			tempFloatBuffer = ImageDataHelpers::ConvertDataType<uint8_t, float>(
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
	m_resourceIdPool.insert(id.m_resourceId);
	Texture2D retVal{ id };
	return retVal;
}


// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
Texture2D TextureManager::Create2DTexture(int _width, int _height, TextureProperties::TextureProps _props) {
	GPUResourceHandle gpuHandle = m_gpuResourceManager.CreateTexture(
		TextureProperties::TextureType::TEXTURE_2D,
		{ _width, _height, 1 },
		_props
	);
	std::shared_ptr<TextureRes> texHandle = std::make_shared<TextureRes>();
	ResourceIdentifier id = m_resourceManager.AddInternalResource(texHandle);
	m_resourceIdPool.insert(id.m_resourceId);
	//LOG_INFO("Allocating 2D Texture of size: [" << _width << ", " << _height << "]");
	Texture2D retVal{ id, gpuHandle };
	return retVal;
}

void TextureManager::Create3DTexture(int _width, int _height, int _depth, TextureProperties::TextureProps _props) {

	GPUResourceHandle gpuHandle = m_gpuResourceManager.CreateTexture(
		TextureProperties::TextureType::TEXTURE_3D,
		{ _width, _height, _depth },
		_props
	);
	// create the handle here.

}

Texture2DArray TextureManager::Create2DArrayTexture(int _width, int _height, int _layers, TextureProperties::TextureProps _props) {
	GPUResourceHandle gpuHandle = m_gpuResourceManager.CreateTexture(
		TextureProperties::TextureType::TEXTURE_2D_ARRAY,
		{ _width, _height, _layers },
		_props
	);
	std::shared_ptr<TextureRes> texHandle = std::make_shared<TextureRes>();
	ResourceIdentifier id = m_resourceManager.AddInternalResource(texHandle);
	m_resourceIdPool.insert(id.m_resourceId);
	//LOG_INFO("Allocating 2D Texture array of size: [" << _width << ", " << _height << "] with " << _layers << " layers.");
	Texture2DArray retVal { id, gpuHandle };
	return retVal;
}

Cubemap TextureManager::CreateCubemapTexture(int _dimensions, TextureProperties::TextureProps _props) {
	GPUResourceHandle gpuHandle = m_gpuResourceManager.CreateTexture(
		TextureProperties::TextureType::CUBEMAP,
		{ _dimensions, _dimensions, 1 },
		_props
	);// move dimensions 
	std::shared_ptr<TextureRes> texHandle = std::make_shared<TextureRes>();
	ResourceIdentifier id = m_resourceManager.AddInternalResource(texHandle);
	m_resourceIdPool.insert(id.m_resourceId);
	//LOG_INFO("Allocating 2D Texture array of size: [" << _width << ", " << _height << "] with " << _layers << " layers.");
	Cubemap retVal{ id, gpuHandle };
	return retVal;
}

Texture2D TextureManager::GetTexture2D(RES_ID _resourceId) {
	auto res = m_resourceManager.GetResource(_resourceId);

	return Texture2D();
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------






