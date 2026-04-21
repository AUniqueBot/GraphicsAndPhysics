#include <arch/systems/sys_render_modules/sys_render_renderPass.h>


void RenderPass::Render() {

	static Material defaultPassMaterial;
	static bool defaultMaterialInit{};
	if (!defaultMaterialInit) {
		// init here.
		
		defaultMaterialInit = true;
	}

	Material* currentRunMaterial = m_passMaterial.get();
	if (currentRunMaterial == nullptr) currentRunMaterial = &defaultPassMaterial;
	

	// run material here..

	glUseProgram(currentRunMaterial->GetShaderProgram());


}


void RenderPass::SetTexture(GLint _texHandle) {
	m_texture = _texHandle;
}

const GLint& RenderPass::GetTexture() const {
	// TODO: insert return statement here
	return m_texture;
}

void RenderPass::SetMaterial(const std::shared_ptr<Material>& _material) {
	m_passMaterial = _material;
}

const std::shared_ptr<Material>& RenderPass::GetMaterial() const {
	return m_passMaterial;
}




