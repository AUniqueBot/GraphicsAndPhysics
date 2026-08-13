#pragma once
#include <arch/resources/res_gpu_resources/res_gpu_resource.h>


class GPU_Buffer : public GPU_Resource {
public:

public:

	void Create() override;
	void Destroy() override;

public:
	void Allocate(size_t size, GLenum _usage = GL_STATIC_DRAW);
	void Upload(const void* _data, size_t _offset = 0);

private:
	size_t m_size	{  };
};