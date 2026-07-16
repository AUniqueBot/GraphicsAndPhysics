#include <arch/resources/res_texture/res_texture2darray.h>



Texture2DArray::Texture2DArray(const TextureIDInfo& _info) : Texture(_info) {
	using namespace TextureProperties;
	m_textureType = TextureType::TEXTURE_2D_ARRAY;
}
