#include <pch.h>
#include <arch/resources/res_texture/res_texture_properties.h>

namespace TextureProperties {

	// Hidden from headers.
	InternalImageDecomposed OpenGL_ToDecomposed(TextureFormat _format) {
		InternalImageDecomposed decomposed{};
		switch (_format) {
		case TextureFormat::R8:
			decomposed.m_pixelDataType = ImageDataType::UINT_8;
			decomposed.m_pixelFormat = ImageChannels::Red;
			break;
		case TextureFormat::RG8:
			decomposed.m_pixelDataType = ImageDataType::UINT_8;
			decomposed.m_pixelFormat = ImageChannels::RG;
			break;
		case TextureFormat::RGB8:
			decomposed.m_pixelDataType = ImageDataType::UINT_8;
			decomposed.m_pixelFormat = ImageChannels::RGB;
			break;
		case TextureFormat::RGBA8:
			decomposed.m_pixelDataType = ImageDataType::UINT_8;
			decomposed.m_pixelFormat = ImageChannels::RGBA;
			break;
		case TextureFormat::SRGB8:
			decomposed.m_pixelDataType = ImageDataType::UINT_8;
			decomposed.m_pixelFormat = ImageChannels::RGB;
			break;
		case TextureFormat::SRGBA8:
			decomposed.m_pixelDataType = ImageDataType::UINT_8;
			decomposed.m_pixelFormat = ImageChannels::RGBA;
			break;
		case TextureFormat::R16F:
			decomposed.m_pixelDataType = ImageDataType::FLOAT_32;
			decomposed.m_pixelFormat = ImageChannels::Red;
			break;
		case TextureFormat::RG16F:
			decomposed.m_pixelDataType = ImageDataType::FLOAT_32;
			decomposed.m_pixelFormat = ImageChannels::RG;
			break;
		case TextureFormat::RGB16F:
			decomposed.m_pixelDataType = ImageDataType::FLOAT_32;
			decomposed.m_pixelFormat = ImageChannels::RGB;
			break;
		case TextureFormat::RGBA16F:
			decomposed.m_pixelDataType = ImageDataType::FLOAT_32;
			decomposed.m_pixelFormat = ImageChannels::RGBA;
			break;

			// depth components.
		case TextureFormat::DEPTH24:
			decomposed.m_pixelDataType = ImageDataType::UINT_8;
			decomposed.m_pixelFormat = ImageChannels::Depth;
			break;
		case TextureFormat::DEPTH32F:
			decomposed.m_pixelDataType = ImageDataType::FLOAT_32;
			decomposed.m_pixelFormat = ImageChannels::Depth;
			break;
		}

		return decomposed;
	}


	GLenum OpenGL_ImageFormatToEnum(TextureFormat _format) {
		switch (_format) {
		case TextureFormat::R8: return GL_R8;
		case TextureFormat::RG8: return GL_RG8;
		case TextureFormat::RGB8: return GL_RGB8;
		case TextureFormat::RGBA8: return GL_RGBA8;
		case TextureFormat::R16F: return GL_R16F;
		case TextureFormat::RG16F: return GL_RG16F;
		case TextureFormat::RGB16F: return GL_RGB16F;
		case TextureFormat::RGBA16F: return GL_RGBA16F;
		case TextureFormat::SRGB8: return GL_SRGB8;
		case TextureFormat::SRGBA8: return GL_SRGB8_ALPHA8;
		case TextureFormat::DEPTH24: return GL_DEPTH_COMPONENT24;
		case TextureFormat::DEPTH32F: return GL_DEPTH_COMPONENT32F;
		default: return GL_INVALID_ENUM;
		}
	}


	int GetUploadDimension(TextureProperties::TextureType _type) {
		using namespace TextureProperties;
		switch (_type)
		{
		case TextureType::TEXTURE_1D:
			return 1;

		case TextureType::TEXTURE_1D_ARRAY:
		case TextureType::TEXTURE_2D:
			return 2;

		case TextureType::TEXTURE_2D_ARRAY:
		case TextureType::TEXTURE_3D:
		case TextureType::CUBEMAP:
		case TextureType::CUBEMAP_ARRAY:
			return 3;

		default:
			throw std::runtime_error("Unsupported texture type.");
		}
	}

}




std::ostream& operator<<(std::ostream& _os, const TextureProperties::TextureType& _type) {
	using namespace TextureProperties;
	switch (_type) {
	case TextureType::TEXTURE_1D:
		_os << "TEXTURE_1D";
		break;
	case TextureType::TEXTURE_1D_ARRAY:
		_os << "TEXTURE_1D_ARRAY";
		break;
	case TextureType::TEXTURE_2D:
		_os << "TEXTURE_2D";
		break;
	case TextureType::TEXTURE_2D_ARRAY:
		_os << "TEXTURE_2D_ARRAY";
		break;
	case TextureType::TEXTURE_3D:
		_os << "TEXTURE_3D";
		break;
	case TextureType::CUBEMAP:
		_os << "TEXTURE_CUBEMAP";
		break;
	case TextureType::CUBEMAP_ARRAY:
		_os << "CUBEMAP_ARRAY";
		break;
	default:
		_os << "UNKNOWN_TEXTURE_TYPE";
		break;
	}

	return _os;
}

std::ostream& operator<<(std::ostream& _os, const TextureProperties::ImageDataType& _type) {
	using namespace TextureProperties;
	switch (_type) {
	case ImageDataType::UINT_8:
		_os << "UNSIGNED_BYTE";
		break;
		//case ImageDataType::INT_8:
			//_os << "BYTE";
			//break;
		//case PixelDataType::UNSIGNED_SHORT:
		//	_os << "UNSIGNED_SHORT";
		//	break;
		//case PixelDataType::SHORT:
		//	_os << "SHORT";
		//	break;
	case ImageDataType::FLOAT_32:
		_os << "FLOAT";
		break;
	default:
		_os << "UNKNOWN_PIXEL_TYPE";
	}
	return _os;
}

std::ostream& operator<<(std::ostream& _os, const TextureProperties::ImageChannels& _type) {
	using namespace TextureProperties;
	switch (_type) {
	case ImageChannels::Depth:
		_os << "DEPTH";
		break;
	case ImageChannels::Red:
		_os << "RED";
		break;
	case ImageChannels::RG:
		_os << "RG";
		break;
	case ImageChannels::RGB:
		_os << "RGB";
		break;
	case ImageChannels::RGBA:
		_os << "RGBA";
		break;
	default:
		_os << "UNKNOWN_PIXEL_FORMAT";
	}
	return _os;
}

std::ostream& operator<<(std::ostream& _os, const TextureProperties::TextureFormat& _type) {
	using namespace TextureProperties;
	switch (_type) {
	case TextureFormat::R8:
		_os << "R8";
		break;
	case TextureFormat::RGB8:
		_os << "RGB8";
		break;
	case TextureFormat::RGBA8:
		_os << "RGBA8";
		break;



	case TextureFormat::SRGB8:
		_os << "SRGB8";
		break;
	case TextureFormat::SRGBA8:
		_os << "SRGBA8";
		break;



	case TextureFormat::R16F:
		_os << "R16F";
		break;
	case TextureFormat::RGB16F:
		_os << "RGB16F";
		break;
	case TextureFormat::RGBA16F:
		_os << "RGBA16F";
		break;


	case TextureFormat::DEPTH24:
		_os << "DEPTH24";
		break;
	case TextureFormat::DEPTH32F:
		_os << "DEPTH32F";
		break;


	default:
		_os << "UNKNOWN_IMAGE_FORMAT";
	}
	return _os;
}

std::ostream& operator<<(std::ostream& _os, const TextureProperties::WrapBehaviour& _type) {
	using namespace TextureProperties;
	switch (_type) {
	case WrapBehaviour::MIRROR_REPEAT:
		_os << "MIRROR_REPEAT";
		break;
	case WrapBehaviour::REPEAT:
		_os << "REPEAT";
		break;
	case WrapBehaviour::TO_BORDER:
		_os << "TO_BORDER";
		break;
	case WrapBehaviour::TO_EDGE:
		_os << "TO_EDGE";
		break;
	default:
		_os << "_UNDEFINED_WRAP_BEHAVIOUR";
	}
	return _os;

}

std::ostream& operator<<(std::ostream& _os, const TextureProperties::FilterBehaviour& _type) {
	using namespace TextureProperties;
	switch (_type) {
	case FilterBehaviour::LINEAR:
		_os << "LINEAR";
		break;
	case FilterBehaviour::NEAREST:
		_os << "NEAREST";
		break;
	default:
		_os << "_UNDEFINED_FILTER_BEHAVIOUR";
	}
	return _os;
}
