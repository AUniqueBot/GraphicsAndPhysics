#pragma once
#include <pch.h>
#include <unordered_set>
#include <arch/resources/res_gpu_resources/res_gpu_resource.h>
#include <arch/resources/res_gpu_resources/res_gpu_buffer.h>
#include <arch/resources/res_mesh_vertexLayout.h>

namespace BindingConstants {
	constexpr const GLuint C_BINDING_POSITION = 0;
	constexpr const GLuint C_BINDING_NORMAL = 1;
	constexpr const GLuint C_BINDING_UV = 2;

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
	void Create() override; 
	void Destroy() override;
	

	// - config -------------------------

	void SetBinding(GLuint _attributeIndex, GLuint _bindingSlot);
	void SetAttribute(GLuint _attributeIndex, Datatype _type, int _componentCount, bool _normalized = false, int _offset = 0);
	void AttachBuffer(GLuint _bindingSlot, const GPU_Buffer& _buffer, int _stride);
	void Clear();
	

	void EnableAttribute(GLuint _attributeIndex);
	void DisableAttribute(GLuint _attributeIndex);

	void UseVAO();


private:
	std::unordered_set<GLuint> m_usedBindings;
	std::unordered_set<GLuint> m_usedAttributes;
};