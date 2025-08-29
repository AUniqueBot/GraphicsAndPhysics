#pragma once
#include <pch.h>

// strict enforce SOA.


class Mesh;
// attribute thing - only worried about how to read data.
struct AttributeProps {
	std::string		m_name;
	unsigned		m_bufferID;
	unsigned		m_bindingPosition;
	GLenum			m_type;
	unsigned		m_featureCount; // how many features per element
	bool			m_normalised;
	bool			m_isActive		{ true };




	void SetAttributeEnabled(bool _isEnabled);
	bool GetAttributeEnabled() const; 
};


class VAOHandler {
private:

public:
	void Init();

	void BindVAO() const;

	void SetVAO(unsigned _vao);

	unsigned GetVAO() const;

	void UseMesh(const Mesh& _mesh);

	static void UnbindVAO();
	static std::vector<GLuint> GenerateBuffers(unsigned _bufferCount);

	void SetData(std::string _name, const Mesh& _mesh);

	void SetData(
		std::string _name, 
		const void* _data, 
		unsigned _dataCount,
		unsigned _typeSize,
		GLenum _dataUsage = GL_STATIC_DRAW
	);


	void FillVertexAttributeData(
		unsigned _bufferID,
		const void* _data,
		unsigned _dataCount,
		unsigned _typeSize,
		GLenum _dataUsage
	);
	
	void FillEmptyVertexAttributeData(
		unsigned _attribPosition, 
		unsigned _typeSize
	);




	// - ebo ----------
	void CreateEBO();
	void SetVertexIndices(const void* _data, unsigned _dataSize);


	void SetAttribute(
		std::string _name,
		GLuint _bindingPosition,
		GLuint _buffer,
		GLenum _type,
		unsigned _featureCount,
		bool _normalised
	);


	unsigned AttributeCount() const;


public:
	void LogDebug() const;


private:
	unsigned m_vao{};
	unsigned m_ebo{};
	std::unordered_map<std::string, AttributeProps> m_attributeBuffers;
};




class VAOManager {
private:
	
	
public:
	void Init();


	void BindVAO(const std::string& _name) const;
	void UnbindVAO() const;


	void AddVAO(const std::string& _name, const VAOHandler& _vao);
	const VAOHandler* GetVAO(const std::string& _name) const;
	VAOHandler* GetVAO(const std::string& _name);
	void RemoveVAO(std::string _name);





private:
	std::unordered_map<std::string, VAOHandler> m_vaoMap{};
	
};