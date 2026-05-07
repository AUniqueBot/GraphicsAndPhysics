#include <arch/resources/res_texture/res_texture2darray.h>

Texture2DArray::Texture2DArray(int _width, int _height, int _layerCount) : 
	Texture(TextureProperties::TextureType::TEXTURE_2D_ARRAY) {
	m_dimensions.x = _width;
	m_dimensions.y = _height;
	m_dimensions.z = _layerCount;
}


void Texture2DArray::SetDimensions(glm::ivec2 _dimensions) {

}
glm::ivec2 Texture2DArray::GetDimensions() const {
	return { m_dimensions.x, m_dimensions.y };
}
void Texture2DArray::SetWidth(int _width) {
	if (m_dimensions.x == _width) return;
	m_dimensions.x = _width;
}
int Texture2DArray::GetWidth() const {
	return m_dimensions.x;
}
void Texture2DArray::SetHeight(int _height) {
	if (m_dimensions.y == _height) return;
	m_dimensions.y = _height;
}
int Texture2DArray::GetHeight() const {
	return m_dimensions.y;
}
void Texture2DArray::SetLayerCount(int _layerCount) {
	if (m_dimensions.z == _layerCount) return;
	m_dimensions.z = _layerCount;
}
int Texture2DArray::GetLayerCount() const {
	return m_dimensions.z;
}


