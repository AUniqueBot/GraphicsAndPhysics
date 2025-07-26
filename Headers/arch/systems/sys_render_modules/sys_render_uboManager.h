#pragma once
#include <pch.h>

class UBO;
class UBOManager;

class UBO {
	friend class UBOManager;
public:
	enum BUFFER_TYPE {
		TRANSFORMS,				// transform buffer - contains camera, projection, and object transform
		MATERIALS,				// buffer for material data.
		LIGHTS,					// buffer for lights
		_COUNT
	};
public:
	void Init();
	inline size_t BufferSize() const;
	inline void BufferSize(size_t _size);


	void BindBuffer();

	void FillBufferData(const void* _data);


private:
	
	BUFFER_TYPE m_bufferType{ _COUNT };
	GLuint m_bufferId{ 0 };
	size_t m_bufferSize{ 0 };

};



class UBOManager {

public:
	void Init();

	void CreateUBO(UBO::BUFFER_TYPE _bufferType, size_t _size = 100);

private:
	std::map<UBO::BUFFER_TYPE, UBO> m_uboMap;
};