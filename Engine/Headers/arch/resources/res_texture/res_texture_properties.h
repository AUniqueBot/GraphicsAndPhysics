#pragma once

namespace TextureProperties {

	enum class TextureType : GLenum {

		TEXTURE_1D = GL_TEXTURE_1D,
		TEXTURE_1D_ARRAY = GL_TEXTURE_1D_ARRAY,
		TEXTURE_2D = GL_TEXTURE_2D,
		TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
		TEXTURE_3D = GL_TEXTURE_3D,
		CUBEMAP = GL_TEXTURE_CUBE_MAP,
		CUBEMAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY,
		_UNDEFINED = 0
	};


	enum class ImageDataType : GLenum {
		UINT_8 = GL_UNSIGNED_BYTE,
		//INT_8 = GL_BYTE,
		//UINT_16 = GL_UNSIGNED_SHORT,
		//INT_16 = GL_SHORT,
		FLOAT_32 = GL_FLOAT
	};

	enum class ImageChannels : GLenum {
		Red = GL_RED,
		RG = GL_RG,
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		Depth = GL_DEPTH_COMPONENT
	};

	enum class TextureFormat : GLenum {
		R8 = GL_R8,

		RG8 = GL_RG8,

		RGB8 = GL_RGB8,
		RGBA8 = GL_RGBA8,

		SRGB8 = GL_SRGB8,
		SRGBA8 = GL_SRGB8_ALPHA8,

		R16F = GL_R16F,
		RG16F = GL_RG16F,
		RGB16F = GL_RGB16F,
		RGBA16F = GL_RGBA16F,

		DEPTH24 = GL_DEPTH_COMPONENT24,
		DEPTH32F = GL_DEPTH_COMPONENT32F
	};




	enum class WrapBehaviour : int {
		REPEAT = GL_REPEAT,
		MIRROR_REPEAT = GL_MIRRORED_REPEAT,
		TO_EDGE = GL_CLAMP_TO_EDGE,
		TO_BORDER = GL_CLAMP_TO_BORDER
	};

	enum class FilterBehaviour : int {
		LINEAR = GL_LINEAR,
		NEAREST = GL_NEAREST,
		_UNDEFINED = GL_INVALID_ENUM

	};

	struct TextureProps {

		int m_mipmapCount{ 1 };
		bool m_autogenerateMipmaps{ false };
		TextureFormat m_internalImageFormat{ TextureFormat::RGBA8 };
		ImageDataType m_pixelDatatype{ ImageDataType::UINT_8 };
		ImageChannels m_pixelFormat{ ImageChannels::RGBA };

		WrapBehaviour m_wrapU{ TextureProperties::WrapBehaviour::REPEAT };
		WrapBehaviour m_wrapV{ TextureProperties::WrapBehaviour::REPEAT };

		FilterBehaviour m_filterMin{ TextureProperties::FilterBehaviour::LINEAR };
		FilterBehaviour m_filterMag{ TextureProperties::FilterBehaviour::LINEAR };
	};



	struct TextureSamplingProperties {
		WrapBehaviour m_wrapU;
		WrapBehaviour m_wrapV;

		FilterBehaviour m_filterMin;
		FilterBehaviour m_filterMag;
	};

	struct ImageUploadData {
		int m_mipLevel;
		glm::ivec3 m_dimensions{};
		void* m_textureData;
		bool m_textureIsFlipped{ false };
	};

	using TextureUploadData = std::optional<std::vector<TextureProperties::ImageUploadData>>;


	namespace OpenGL {
		// specifically for images a depth component image is not possible so it's ignored.
		inline GLenum ResolveChannels(TextureProperties::ImageChannels _channels) {
			using namespace TextureProperties;
			return
				_channels == ImageChannels::Red ? GL_RED :
				_channels == ImageChannels::RG ? GL_RG :
				_channels == ImageChannels::RGB ? GL_RGB :
				_channels == ImageChannels::RGBA ? GL_RGBA :
				GL_DEPTH_COMPONENT;
		}

		inline GLenum ResolveDataType(TextureProperties::ImageDataType _dataType) {
			using namespace TextureProperties;
			return
				_dataType == ImageDataType::UINT_8 ? GL_UNSIGNED_BYTE :
				//_dataType == ImageDataType::UINT_16 ? GL_UNSIGNED_SHORT :
				GL_FLOAT;
		}

	}


}

namespace TextureProperties {

	struct InternalImageDecomposed {
		ImageDataType m_pixelDatatype;
		ImageChannels m_pixelFormat;
	};
	// Hidden from headers.
	InternalImageDecomposed OpenGL_ToDecomposed(TextureFormat _format);
	GLenum OpenGL_ImageFormatToEnum(TextureFormat _format);
	int GetUploadDimension(TextureProperties::TextureType _type);
}

std::ostream& operator<<(std::ostream& _os, const TextureProperties::TextureType& _type);
std::ostream& operator<<(std::ostream& _os, const TextureProperties::ImageDataType& _type);
std::ostream& operator<<(std::ostream& _os, const TextureProperties::ImageChannels& _type);
std::ostream& operator<<(std::ostream& _os, const TextureProperties::TextureFormat& _type);
std::ostream& operator<<(std::ostream& _os, const TextureProperties::WrapBehaviour& _type);
std::ostream& operator<<(std::ostream& _os, const TextureProperties::FilterBehaviour& _type);