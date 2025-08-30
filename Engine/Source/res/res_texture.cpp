#include <pch.h>
#include <arch/resources/res_texture.h>




bool Texture::Load() {
		
	glGenTextures(1, &m_textureID);
	if (!BindTexture()) {
		std::stringstream ss;
		ss << m_resPath.filename() << ":: Failed to generate texture memory" << std::endl;
		LOG_ERROR(ss.str());
	}

	
	// do texture things here.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapU);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapV);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filterU);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filterV);
	// unsigned char* imgData = stbi(m_resPath.str().c_str(), m_dimensions[0], m_dimensions[1], &m_channelCount, 0);
	// 

	return LoadImage();
}

void Texture::SetWrappingBehavior(WRAP_BEHAVIOUR _h, WRAP_BEHAVIOUR _v) {
	SetWrappingBehaviorH(_h);
	SetWrappingBehaviorV(_v);

}

void Texture::SetWrappingBehaviorH(WRAP_BEHAVIOUR _setting) {
	m_wrapUDirty = true;
	m_wrapU = _setting;
}

void Texture::SetWrappingBehaviorV(WRAP_BEHAVIOUR _setting) {
	m_wrapVDirty = true;
	m_wrapV = _setting;
}



void Texture::SetFilterBehavior(FILTER_BEHAVIOUR _h, FILTER_BEHAVIOUR _v) {
	SetFilterBehaviorH(_h);
	SetFilterBehaviorV(_v);
}

void Texture::SetFilterBehaviorH(FILTER_BEHAVIOUR _setting) {
	m_filterUDirty = true;
	m_filterU = _setting;
}

void Texture::SetFilterBehaviorV(FILTER_BEHAVIOUR _setting) {
	m_filterVDirty = true;
	m_filterV = _setting;
}

void Texture::SetMipmapCount(int _setting) {
	m_mipmapCount = _setting;
}

void Texture::FlipVertically(bool _setting) {
	m_flipImage = _setting;
}

bool Texture::LoadImage() {
	
	unsigned char* imgData		{ nullptr };
	// stbi_set_flip_vertically_on_load(m_flipImage);
	if (!imgData) {
		std::stringstream ss;
		ss << m_resPath.filename() << ":: Failed to load texture" << std::endl;
		LOG_ERROR(ss.str());
		return false;
	}
	glTexImage2D(
		GL_TEXTURE_2D, 
		0, 
		static_cast<GLint>(m_imageFormatI),
		m_dimensions[0],
		m_dimensions[1],
		0,
		static_cast<GLint>(m_imageFormat),
		GL_UNSIGNED_BYTE,
		imgData
	);

	glGenerateMipmap(GL_TEXTURE_2D);


	
	return true;
}

void Texture::UpdateResourceParameters() {
	if (!BindTexture()) return;
	// adjust params here.


	if (m_wrapUDirty) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapU);
		m_wrapUDirty = false;
	}

	if (m_wrapVDirty) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapV);
		m_wrapVDirty = false;
	}

	if (m_filterUDirty) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filterU);
		m_filterUDirty = false;
	}


	if (m_filterVDirty) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filterV);
		m_filterVDirty = false;
	}



}
