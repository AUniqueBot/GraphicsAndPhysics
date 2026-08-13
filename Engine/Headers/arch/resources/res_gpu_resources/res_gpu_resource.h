#pragma once
#include <pch.h>
#include <arch/resources/res_gpu_resources/res_gpu_gpuhandle.h>


class GPU_Resource {
public:
	virtual void Create() = 0;
	virtual void Destroy() = 0;
	inline const GPUHandle& GetHandle() const { return m_handle; }
protected:
	GPUHandle m_handle;
	bool m_isCreated	{};
};