
#include <graphics/context/gfx_graphicsContext_opengl_4_6.h>

void GraphicsContext_OpenGL4_6::DrawImpl(
	const GraphicsContextTypes::DrawArgs& _args
) {
	
}

void GraphicsContext_OpenGL4_6::DrawIndexedImpl(
	const GraphicsContextTypes::DrawIndexedArgs& _args
) {

	uint32_t firstIdx { _args.indexOffset };
	uint32_t idxCount { _args.indexCount };

	glDrawElements(GL_TRIANGLES, idxCount, GL_UNSIGNED_INT, nullptr);
}
