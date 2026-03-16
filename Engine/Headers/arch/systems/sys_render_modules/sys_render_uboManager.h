#pragma once
#include <pch.h>
#include <arch/datatypes/type_sparseSet.h>




class UBO {

public:
	void Init();
	
	size_t BufferSize() const;
	void BufferSize(size_t _size);

	GLuint GetID() const { return m_bufferId; };

	void BindBuffer() const;
	static void UnbindBuffer();
	void SetBindingIndex(GLuint _bindingIdx);
	const GLuint& GetBindingIndex() const;

	void FillBufferData(const void* _data) const;

private:
	
	GLuint m_bufferId							{ 0 };
	size_t m_bufferSize							{ 0 };
	GLuint m_bindingIndex						{ 0 };

	inline static GLuint m_currentBoundId		{};

};



namespace DefaultUBOs {
	constexpr const char* DEFAULTBUFFER_COMMON	{ "COMMON" };
	constexpr const char* DEFAULTBUFFER_OBJECT	{ "OBJECT" };
	constexpr const char* DEFAULTBUFFER_LIGHTS	{ "LIGHTS" };
	constexpr const char* DEFAULTBUFFER_SHADOW	{ "SHADOW" };
	constexpr const int BINDINGOFFSET			{ 4 };
}



class UBOManager {
public:

public:
	void Init();
	void CreateUBO(std::string _bufferName, int _bindIndex, size_t _size);

	UBO* GetUBO(std::string _bufferName);
	const UBO* GetUBO(std::string _bufferName) const;


	void RemoveUBO(std::string _bufferName);
	bool HasUBO(std::string _bufferName) const;

private:
	SparseSet<std::string, UBO> m_uboDatabase;

};