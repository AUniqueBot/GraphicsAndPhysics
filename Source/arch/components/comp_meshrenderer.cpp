
#include <pch.h>
#include <arch/components/comp_meshrenderer.h>


MeshRenderer::MeshRenderer() {
	SetComponentID(ComponentType::RENDER);
}

void MeshRenderer::Init() {
	//

	

}

void MeshRenderer::End() {
	//
}

int MeshRenderer::VAO() const {

	return 0;
}

int MeshRenderer::VBO() const {
	return 0;
}
