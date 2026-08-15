#pragma once
#include <arch/resources/res_gpu_resources/res_gpu_resource.h>


class GPU_Buffer : public GPU_Resource {
public:

public:

	void Create() override;
	void Destroy() override;

public:
	/*!
		@brief allocates a buffer for use.
	*/
	void Allocate(size_t size, GLenum _storageFlags = GL_DYNAMIC_STORAGE_BIT);
	void SetUsage(GLenum _usage);
	void SetSize(size_t _size);
	void Upload(const void* _data, size_t _size, size_t _offset = 0);

private:
	size_t m_size	{  };
	GLenum m_usage	{ GL_INVALID_ENUM };
};