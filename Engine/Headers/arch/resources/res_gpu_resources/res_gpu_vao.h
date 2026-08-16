#pragma once
#include <pch.h>
#include <unordered_set>
#include <arch/resources/res_gpu_resources/res_gpu_resource.h>
#include <arch/resources/res_gpu_resources/res_gpu_buffer.h>
#include <arch/resources/res_mesh_vertexLayout.h>

namespace BindingConstants {
	inline constexpr const GLuint C_BINDING_POSITION = 0;
	inline constexpr const GLuint C_BINDING_NORMAL = 1;
	inline constexpr const GLuint C_BINDING_UV = 2;

}



using BufferID = GLuint;

// VAO
class GPU_VertexArrayObject : public GPU_Resource {
public:
	enum Datatype {
		INT,
		FLOAT
	};
public:

	void Bind() const;

	void Create() override; 
	void Destroy() override;
	

	// - config -------------------------
	void SetupAttributes(const VertexLayout& _layout);
	void SetBinding(GLuint _attributeIndex, GLuint _bindingSlot);
	void SetAttribute(GLuint _attributeIndex, GLenum _type, int _componentCount, bool _normalized = false, int _offset = 0);
	void EnableAttribute(GLuint _attributeIndex);
	void DisableAttribute(GLuint _attributeIndex);

	void AttachBuffer(GLuint _bindingSlot, const GPU_Buffer& _buffer, int _stride);
	int AliasToBinding(std::string _alias) const;
	int AliasToAttribute(std::string _alias) const;
	void Clear();
	


	void UseVAO();

private:
	std::unordered_set<GLuint> m_usedBindings;
	std::unordered_set<GLuint> m_usedAttributes;
	std::unordered_map<std::string, GLuint> m_aliasToBindings;
	std::unordered_map<std::string, GLuint> m_aliasToAttributes;
};