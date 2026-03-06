#pragma once
#include <pch.h>





class UBO {

public:
	void Init();
	
	size_t BufferSize() const;
	void BufferSize(size_t _size);

	GLuint GetID() const { return m_bufferId; };

	void BindBuffer() const;
	static void UnbindBuffer();
	void SetBindingIndex(GLuint _bindingIdx);

	void FillBufferData(const void* _data) const;

private:
	
	GLuint m_bufferId							{ 0 };
	size_t m_bufferSize							{ 0 };
	GLuint m_bindingIndex						{ 0 };

	inline static GLuint m_currentBoundId		{};

};



class UBOManager {
public:
	enum BUFFER_TYPE {
		TRANSFORMS,				// transform buffer - contains camera, projection, and object transform
		MATERIALS,				// buffer for material data.
		LIGHTS,					// buffer for lights
		SHADOWS,				// shadow map buffer.
		_COUNT
	};
public:
	void Init();

	void CreateUBO(BUFFER_TYPE _bufferType, size_t _size = 100);

	UBO* GetUBO(BUFFER_TYPE _bufferType);
	const UBO* GetUBO(BUFFER_TYPE _bufferType) const;

private:
	std::map<BUFFER_TYPE, UBO> m_uboMap;
};