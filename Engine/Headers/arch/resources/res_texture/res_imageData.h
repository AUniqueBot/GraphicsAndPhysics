#pragma once
#include <pch.h>
#include <arch/resources/res_texture/res_texture_properties.h>

struct ImageData {
private:
	std::vector<std::byte> m_data;
public:
	glm::ivec2 m_dimensions;
	TextureProperties::ImageChannels m_channels;
	TextureProperties::ImageDataType m_dataType;

	glm::ivec2 m_originalDimensions;
	TextureProperties::ImageDataType m_originalDatatype;
	TextureProperties::ImageChannels m_originalChannels;
	ImageData() = default;
	ImageData(
		const void* _src,
		glm::ivec2 _dims,
		TextureProperties::ImageChannels _ogChannels,
		TextureProperties::ImageDataType _dataType
	);

	void SetImageData(const void* _src);
	std::byte* GetImageData();


	ImageData(const ImageData&) = default;
	ImageData& operator=(const ImageData&) = default;

	size_t GetImagePixelCount() const;
	size_t GetComponentCount() const;

	static ImageData LoadImage(const std::filesystem::path& _path);

		
	template <typename T>
	inline const T* GetData() const {
		return reinterpret_cast<const T*>(m_data.data());
	}

};

namespace ImageDataHelpers {
	size_t GetPixelCount(glm::ivec2 _dimensions);
	size_t GetByteSize(TextureProperties::ImageDataType _dataType);
	size_t GetChannelCount(TextureProperties::ImageChannels _channels);


	template <typename T>
	static std::vector<T> ConvertRGBAToOtherChannel(
		const T* _data,
		size_t _pixelCount,
		TextureProperties::ImageChannels _destinationChannel
	) {
		size_t pixelCount{ _pixelCount };
		size_t newChannelSize{ GetChannelCount(_destinationChannel) };
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