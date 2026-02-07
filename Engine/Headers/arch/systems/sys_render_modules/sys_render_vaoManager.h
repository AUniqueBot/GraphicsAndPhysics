#pragma once
#include <pch.h>
#include <arch/resources/res_mesh_vertexLayout.h>
// strict enforce SOA.


class Mesh;
// attribute thing - only worried about how to read data.
struct AttributeProps : public VertexAttributeDesc {
	unsigned		m_bufferID;
	unsigned		m_bindingPosition;
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


	std::string SetupAttributes(const VertexLayout& _layout);


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

	void SetAttributeEnabled(std::string _name, bool _enabled = true);


	unsigned AttributeCount() const;


public:
	void LogDebug() const;
private:

	const Mesh* m_currentBoundMesh	{};
private:

	unsigned m_vaoId{};
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