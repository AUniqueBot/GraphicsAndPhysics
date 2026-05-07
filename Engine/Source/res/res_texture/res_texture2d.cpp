#include <arch/resources/res_texture/res_texture2d.h>

Texture2D::Texture2D(int _width, int _height) : Texture(TextureProperties::TextureType::TEXTURE_2D) {
	m_dimensions.x = _width;
	m_dimensions.y = _height;
}

void Texture2D::SetDimensions(glm::ivec2 _dimensions) {
	SetWidth(_dimensions.x);
	SetHeight(_dimensions.y);
}

glm::ivec2 Texture2D::GetDimensions() const {
	return { m_dimensions.x, m_dimensions.y };
}

void Texture2D::SetWidth(int _width) {
	if (_width == m_dimensions.x) return;
	m_dimensions.x = _width;
	m_reallocateDirty = true;
}

int Texture2D::GetWidth() const {
	return m_dimensions.x;
}
void Texture2D::SetHeight(int _height) {
	if (_height== m_dimensions.x) return;
	m_dimensions.x = _height;
	m_reallocateDirty = true;

}
int Texture2D::GetHeight() const {
	return m_dimensions.y;
}

//void Texture2D::Upload(TextureProperties::TextureUploadData  _imageData) {
//
//	int width = m_dimensions.x;
//	int height = m_dimensions.y;
//	int mipCount = m_textureProperties.m_mipmapCount;
//	bool autogenMips = m_textureProperties.m_autogenerateMipmaps;
//	bool hasExternalData = _imageData != std::nullopt; // to replace with a std::vector<void*>
//	
//	if (hasExternalData) {
//		if (m_textureType != ) {
//
//			
//		}
//		if (!autogenMips && _imageData.value().size() != mipCount) {
//			throw std::runtime_error("mismatch mip count and image data");
//		}
//		
//
//	}
//
//	GLenum internalImageFormat = static_cast<GLenum>(m_textureProperties.m_internalImageFormat);
//	GLenum pixelType = static_cast<GLenum>(m_textureProperties.m_pixelDataType);
//
//	if (mipCount > 1) {
//		void* imageData = nullptr;
//		for (int mipLevel = 0; mipLevel < mipCount; ++mipLevel) {
//			int mipWidth = width >> mipLevel;
//			int mipHeight = height >> mipLevel;
//			if (hasExternalData) {
//				const auto& data = _imageData.value()[mipLevel];
//				if (data.m_dimensions.x != mipWidth || data.m_dimensions.y != mipHeight) {
//					std::stringstream ss;
//					ss << "provided mip width does not match current mip width. \n";
//					ss << "provided dimensions: " << glm::ivec2(data.m_dimensions.x, data.m_dimensions.y)<< " || ";
//					ss << "expected dimensions: " << glm::ivec2(mipWidth, mipHeight)<< "\n";
//					ss << "current mip level: " << mipLevel;
//					throw std::runtime_error(ss.str());
//				}
//				imageData = data.m_textureData;
//			}
//			glTextureSubImage2D(
//				m_textureId, 
//				mipLevel, 0, 0, 
//				mipWidth, mipHeight, 
//				internalImageFormat, 
//				pixelType, 
//				imageData
//			);
//		}
//	}
//	else {
//		void* imageData = nullptr;
//		if (hasExternalData) {
//			const auto& data = _imageData.value()[0];
//			if (data.m_dimensions.x != width || data.m_dimensions.y != height) {
//				std::stringstream ss;
//				ss << "provided image width does not match texture width. \n";
//				ss << "provided dimensions: " << glm::ivec2(data.m_dimensions.x, data.m_dimensions.y) << " || ";
//				ss << "expected dimensions: " << glm::ivec2(width, height) << "\n";
//				throw std::runtime_error(ss.str());
//			}
//			imageData = data.m_textureData;
//		}
//
//		glTextureSubImage2D(
//			m_textureId,
//			0, 0, 0,
//			width, height,
//			internalImageFormat,
//			pixelType,
//			imageData
//		);
//		if (autogenMips) {
//			glGenerateTextureMipmap(m_textureId);
//		}
//
//	}
//}
