#pragma once
#include <graphics/resources/gfx_gpuresource.h>



// for creation of a resource.
enum class BufferUsage : uint32_t {
	None = 0,
	Vertex = 1 << 0,
	Index = 1 << 1,
	Uniform = 1 << 2,
	Storage = 1 << 3,
	Indirect = 1 << 4
};

enum class BufferCPUAccess : uint8_t {
	None,
	Update,
	MapWrite
};

struct GPUBufferDesc {
	size_t size = 0;
	BufferUsage usage = BufferUsage::None;
	BufferCPUAccess access = BufferCPUAccess::Update;
};



class GPUBuffer : public GPUResource {
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


	size_t GetSize() const;
private:

	virtual void CreateImpl();
	virtual void DestroyImpl();


	size_t m_size	{  };
	GLenum m_usage	{ GL_INVALID_ENUM };
};