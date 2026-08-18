#pragma once 
#include <arch/resources/res_texture/res_texture.h>


class Cubemap : public Texture {
public:
	Cubemap(ResourceIdentifierArg _resHandle = std::nullopt, GPUResourceHandle _resGpuHandle = {});
	Cubemap(const Cubemap&) = default;
	Cubemap& operator=(const Cubemap&) = default;

	int GetWidth() const;
	void SetWidth(int _width);

};