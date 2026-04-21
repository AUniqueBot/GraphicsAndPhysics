#pragma once
#include <arch/resources/res_material.h>
#include <arch/systems/sys_render_modules/sys_render_renderTargetManager.h>


class RenderPass {

	

public:
	// take in texture

	void Render();

	void SetTexture(GLint _texHandle);
	const GLint& GetTexture() const;

	void SetMaterial(const std::shared_ptr<Material>& material);
	const std::shared_ptr<Material>& GetMaterial() const;

private:

private:
	GLint m_texture{};
	std::shared_ptr<Material> m_passMaterial{};

};