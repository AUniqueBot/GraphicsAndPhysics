#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>
#include <arch/resources/res_texture/res_imagedata.h>


namespace ImageDataHelpers {

	size_t GetPixelCount(glm::ivec2 _dimensions) {
		return static_cast<size_t>(_dimensions.x) * _dimensions.y;
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

}



ImageData::ImageData(
	const void* _src,
	glm::ivec2 _dims,
	TextureProperties::ImageChannels _ogChannels,
	TextureProperties::ImageDataType _dataType
)
	: m_dimensions(_dims),
	m_channels(TextureProperties::ImageChannels::RGBA),
	m_dataType(_dataType),

	m_originalDimensions(_dims),
	m_originalChannels(_ogChannels),
	m_originalDatatype(_dataType)
{
	SetImageData(_src);
}

void ImageData::SetImageData(const void* _src) {
	m_data.clear();
	using namespace ImageDataHelpers;
	size_t size = GetPixelCount(m_dimensions) * GetByteSize(m_dataType) * GetChannelCount(m_channels);
	m_data.resize(size);
	std::memcpy(m_data.data(), _src, size);
}

std::byte* ImageData::GetImageData() {
	return (m_data.data());
}

size_t ImageData::GetImagePixelCount() const {
	using namespace ImageDataHelpers;
	return GetPixelCount(m_dimensions);
}
size_t ImageData::GetComponentCount() const {
	return GetImagePixelCount() * static_cast<size_t>(m_channels);
}

ImageData ImageData::LoadImage(const std::filesystem::path& _path) {

	glm::ivec2 dimensions{ 0, 0 };
	int channelCount{};

	bool isHDR = stbi_is_hdr(_path.string().c_str());
	void* data = isHDR ?
		reinterpret_cast<void*>(stbi_loadf(_path.string().c_str(), &dimensions.x, &dimensions.y, &channelCount, 4)) : // hdr
		reinterpret_cast<void*>(stbi_load(_path.string().c_str(), &dimensions.x, &dimensions.y, &channelCount, 4));   // sdr
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

