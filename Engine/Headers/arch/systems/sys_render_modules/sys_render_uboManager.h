#pragma once
#include <pch.h>





class UBO {

public:
	enum BUFFER_TYPE {
		TRANSFORMS,				// transform buffer - contains camera, projection, and object transform
		MATERIALS,				// buffer for material data.
		LIGHTS,					// buffer for lights
		_COUNT
	};
public:
	void Init();
	
	void BufferType(BUFFER_TYPE _type);
	BUFFER_TYPE BufferType() const;
	size_t BufferSize() const;
	void BufferSize(size_t _size);

	GLuint GetID() const { return m_bufferId; };

	void BindBuffer();
	void SetBinding(GLuint _program);

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

	UBO* GetUBO(UBO::BUFFER_TYPE _bufferType);
	const UBO* GetUBO(UBO::BUFFER_TYPE _bufferType) const;

private:
	std::map<UBO::BUFFER_TYPE, UBO> m_uboMap;
};