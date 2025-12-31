#include <Widgets/UIWidget_Viewport.h>
#include <arch/systems/sys_render_modules/sys_render_viewport.h>
#include <imgui.h>



UIWidget_Viewport::UIWidget_Viewport(std::string _widgetName, std::shared_ptr<Viewport> _target)
	: UIWidget(_widgetName), m_viewportPointer{_target} {

}

void UIWidget_Viewport::Init() {

}

void UIWidget_Viewport::Draw() const {
	// only contains the render target.
	using namespace ImGui;
	if (!m_viewportPointer) return;
	Viewport & vp = *m_viewportPointer.get();
	if (!vp.GetRenderTarget()) return;
	RenderTarget& rt = *vp.GetRenderTarget();
	const glm::ivec2& sizeGL = rt.GetSize();
	ImVec2 sizeIm = { static_cast<float>(sizeGL.x), static_cast<float>(sizeGL.y) };
	ImGui::Image((void*)(rt.FBO()), sizeIm);

}

void UIWidget_Viewport::Exit() {
}

void UIWidget_Viewport::ResizeCallback() {
	// get the new window resolution.
	if (!m_viewportPointer || !m_viewportPointer->GetRenderTarget()) return;
	m_viewportPointer->GetRenderTarget()->Resize(1, 2);
}
