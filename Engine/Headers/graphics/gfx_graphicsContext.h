#pragma once
#include <pch.h>


namespace GraphicsContextTypes {
	using BindSlot = int32_t;




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

	void SetViewport(glm::ivec2 _offset, glm::ivec2 _size);
	void SetScissor(glm::ivec2 _offset, glm::ivec2 _size);

	void Clear();


	inline void Draw(const GraphicsContextTypes::DrawArgs& _args) { DrawImpl(_args); }
	inline void DrawIndexed(const GraphicsContextTypes::DrawIndexedArgs& _args) { DrawIndexedImpl(_args); }

	//
protected:
	virtual void DrawImpl(const GraphicsContextTypes::DrawArgs& _args) = 0;
	virtual void DrawIndexedImpl(const GraphicsContextTypes::DrawIndexedArgs& _args) = 0;





};