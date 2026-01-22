#include <Widgets/UIWidget_Compositor.h>
#include <arch/systems/sys_render.h>


UIWidget_Compositor::UIWidget_Compositor(std::string _widgetName) : UIWidget(_widgetName) {
}

UIWidget_Compositor::~UIWidget_Compositor() {
}

void UIWidget_Compositor::Init() {
	Core& c = Core::GetInstance(); // only on init.
	auto* rsp{ c.GetSystem<RenderSystem>() };
	if (c.GetSystem<RenderSystem>()) {
		RenderSystem& rs{ *rsp };
		m_compositor = &rs.GetCompositor();
	}
}

void UIWidget_Compositor::Draw() {
	if (!m_compositor) return;

	// on left bring up available nodes.
	// on right show the editor.
	ImNodes::BeginNodeEditor();		
	

	ImNodes::EndNodeEditor();

}

void UIWidget_Compositor::Update()
{
}

void UIWidget_Compositor::Exit()
{
}
