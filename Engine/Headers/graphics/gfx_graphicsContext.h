#pragma once
#include <pch.h>


using RenderTargetID = int32_t;



class GraphicsContext {

public:
	void BindResource();
	void BindPipeline();
	void BindRenderTarget();//??

	void SetViewport(glm::ivec2 _offset, glm::ivec2 _size);
	void SetScissor(glm::ivec2 _offset, glm::ivec2 _size);

	void Clear();


	void Draw();
	void DrawIndexed();

	//
};