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





private:
	UBO m_lightUBO{};	
	UBO m_transformUBO{};
	UBO m_materialUBO{};

};