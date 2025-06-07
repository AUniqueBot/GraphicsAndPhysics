
#include <pch.h>
#include <arch/components/comp_meshrenderer.h>


MeshRenderer::MeshRenderer() {
	SetComponentID(ComponentType::RENDER);
}

void MeshRenderer::Init() {

}

void MeshRenderer::End() {
	//
}

int MeshRenderer::VAO() const {
	return m_mesh.VAO();
}

int MeshRenderer::VBO() const {
	return 0;
}
