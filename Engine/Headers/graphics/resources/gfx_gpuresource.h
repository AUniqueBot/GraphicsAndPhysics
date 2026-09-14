#pragma once
#include <pch.h>
#include <graphics/resources/gfx_gpuHandle.h>


class GPUResource {
public:
	virtual void Create() = 0;
	virtual void Destroy() = 0;
	inline const GPUHandle& GetHandle() const { return m_handle; }
protected:
	GPUHandle m_handle;
	bool m_isCreated	{};
};