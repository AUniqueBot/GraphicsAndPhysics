#include <arch/resources/res_texture/res_cubemap.h>


Cubemap::Cubemap(
	ResourceIdentifierArg _resHandle, 
	GPUResourceHandle _resGpuHandle
) : Texture(_resHandle, _resGpuHandle) {

}

int Cubemap::GetWidth() const {
	const GPU_Texture& tex = GetGPUTexture();
	return tex.GetX();
}

void Cubemap::SetWidth(int _width) {
	GPU_Texture& tex = GetGPUTexture();
	tex.SetX(_width);
}
