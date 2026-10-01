#pragma once
#include <pch.h>
#include <graphics/resources/gfx_resourcelist.h>

namespace GraphicsContextTypes {
	using BindSlot = int32_t;
	

	enum class VertexFormat : uint32_t {
		Float1,
		Float2,
		Float3
	};


	struct VertexAttribute {
		uint32_t location;
		VertexFormat format;
		uint32_t offset;
		uint32_t binding;
	};

	struct VertexBinding {
		uint32_t binding;
		uint32_t stride;
	};

	struct VertexLayoutDesc {
		std::vector<VertexAttribute> attributes;
		std::vector<VertexBinding> bindings;
	};
	struct VertexInput {
		uint32_t bufferHandle; // makes no sense.
		uint32_t indexHandle;
		VertexLayoutDesc vertexLayout;
	};



	struct DrawArgs {
		uint32_t vertexOffset;
		uint32_t vertexCount;
		uint32_t instanceStart;
		uint32_t instanceCount;
	};

	struct DrawIndexedArgs {
		uint32_t indexOffset;
		uint32_t indexCount;
		uint32_t instanceStart;
		uint32_t instanceCount;
	};




};



class GraphicsContext {

public:
	void BindResource();
	void BindPipeline();
	void BindRenderTarget();//??

	void BindVertexInput();
	void CreateVertexLayout();

	void UpdateBuffer(GPUBuffer& _toFill, void* _data, size_t _size);

	void BindUniformBuffer(int _index, GPUBuffer& _buffer);

	void SetViewport(glm::ivec2 _offset, glm::ivec2 _size);
	void SetScissor(glm::ivec2 _offset, glm::ivec2 _size);

	void Clear();


	inline void Draw(const GraphicsContextTypes::DrawArgs& _args) { DrawImpl(_args); }
	inline void DrawIndexed(const GraphicsContextTypes::DrawIndexedArgs& _args) { DrawIndexedImpl(_args); }

	//
protected:

	virtual void UpdateBufferImpl(GPUBuffer& _toFill, void* _data, size_t _size) = 0;
	
	virtual void DrawImpl(const GraphicsContextTypes::DrawArgs& _args) = 0;
	virtual void DrawIndexedImpl(const GraphicsContextTypes::DrawIndexedArgs& _args) = 0;

}; 