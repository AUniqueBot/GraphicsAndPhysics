#include <pch.h>
#include <arch/resources/res_texture.h>
#include <stb_image.h>


namespace TextureCreation {
	Texture CreateTexture(
		const TextureInfo& _info,
		const void* _data
	) {
		Texture tex{};
		GLuint texId{};

		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexImage2D(
			GL_TEXTURE_2D, 
			0, 
			_info.m_internalFormat, 
			_info.m_resolution.x, _info.m_resolution.y, 
			0, 
			_info.m_format, _info.m_type, 
			_data
		);
		glBindTexture(GL_TEXTURE_2D, 0);
		

		return tex;
	}
	Texture CreateTextureFromFile(
		const std::filesystem::path& path
	) {
		TextureInfo texInfo{};
		int channels{};
		unsigned char* imgData { stbi_load(path.string().c_str(), &texInfo.m_resolution.x, &texInfo.m_resolution.y, &channels, 0) };
		return CreateTexture(texInfo, imgData);
	}
}





bool Texture::BindTexture() const {
	if (m_textureID != 0) glBindTexture(GL_TEXTURE_2D, m_textureID);
	return m_textureID != 0;
}

void Texture::SetWrappingBehavior(TextureCreation::WrapBehaviour _u, TextureCreation::WrapBehaviour _v) {
	SetWrappingBehaviorU(_u);
	SetWrappingBehaviorV(_v);

}

void Texture::SetWrappingBehaviorU(TextureCreation::WrapBehaviour _setting) {
	m_wrapU = _setting;
}

void Texture::SetWrappingBehaviorV(TextureCreation::WrapBehaviour _setting) {
	m_wrapV = _setting;
}

std::pair<
	TextureCreation::WrapBehaviour, 
	TextureCreation::WrapBehaviour
> Texture::GetWrappingBehaviour() const {
	return { GetWrappingBehaviourU(), GetWrappingBehaviourV() };
}

const TextureCreation::WrapBehaviour& Texture::GetWrappingBehaviourU() const {
	return m_wrapU;
}

const TextureCreation::WrapBehaviour& Texture::GetWrappingBehaviourV() const {
	return m_wrapV;
}



void Texture::SetFilterBehavior(TextureCreation::FilterBehaviour _h, TextureCreation::FilterBehaviour _v) {
	SetFilterBehaviorU(_h);
	SetFilterBehaviorV(_v);
}

void Texture::SetFilterBehaviorU(TextureCreation::FilterBehaviour _setting) {
	m_filterU = _setting;
}

void Texture::SetFilterBehaviorV(TextureCreation::FilterBehaviour _setting) {
	m_filterV = _setting;
}

std::pair<
	TextureCreation::FilterBehaviour, 
	TextureCreation::FilterBehaviour
> Texture::GetFilterBehaviour() const {
	return { GetFilterBehaviourU(), GetFilterBehaviourV() };
}

const TextureCreation::FilterBehaviour& Texture::GetFilterBehaviourU() const {
	return m_filterU;
}

const TextureCreation::FilterBehaviour& Texture::GetFilterBehaviourV() const {
	return m_filterV;
}

void Texture::SetMipmapCount(int _setting) {
	m_mipmapCount = _setting;
}

void Texture::FlipVertically(bool _setting) {
	m_flipImage = _setting;
}

void Texture::UpdateResourceParameters() {
	if (!BindTexture()) return;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(m_wrapU));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(m_wrapV));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(m_filterU));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(m_filterV));
}


