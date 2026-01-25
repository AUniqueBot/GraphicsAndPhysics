#include <Widgets/UIWidget_Compositor.h>
#include <arch/systems/sys_render.h>


UIWidget_Compositor::UIWidget_Compositor(std::string _widgetName) : UIWidget(_widgetName) {
	m_flags = { ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar };
}

UIWidget_Compositor::~UIWidget_Compositor() {
}

void UIWidget_Compositor::Init() {
	Core& c = Core::GetInstance(); // only on init.
	auto* rsp{ c.GetSystem<RenderSystem>() };
	if (c.GetSystem<RenderSystem>()) {
		RenderSystem& rs{ *rsp };
		m_compositor = &rs.GetCompositor();
		m_viewportManager = &rs.GetViewportManager();
	}
}

void UIWidget_Compositor::Draw() {
	if (!m_compositor) return;	// on left bring up available nodes.
	Compositor& comp{ *m_compositor };
	// on right show the editor.
	MenuBar();

	float totalWidth { ImGui::GetContentRegionAvail().x };
	static float menuWidth			{ 240.f };
	constexpr float minMenuWidth	{ 120.f };
	menuWidth = glm::clamp(
		menuWidth,
		minMenuWidth,
		totalWidth - minMenuWidth - 4.f
	);



	if (ImGui::BeginChild("Compositor Menu", ImVec2(menuWidth, 0), ImGuiChildFlags_ResizeX)) {
		auto& list{ comp.GetNodeFactory().GetNodeDefinitionsList() };
		for (const CompositionNodeMetadata& data : list) {
			ImGui::Selectable(data.m_nodeName.c_str(), false);
			if (ImGui::BeginDragDropSource()) {
				ImGui::SetDragDropPayload("CompositorNodeMetadata", data.m_nodeName.c_str(), data.m_nodeName.size() + 1);
				ImGui::Text(data.m_nodeName.c_str());
				ImGui::EndDragDropSource();
			}
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();
	CompositorGrid();
	 
}

void UIWidget_Compositor::CompositorGrid() {
	ImNodes::BeginNodeEditor();
	
	ImGui::InvisibleButton("##NodeEditor", ImGui::GetContentRegionAvail());
	if (ImGui::BeginDragDropTarget()) {

		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CompositorNodeMetadata");
		if (payload && m_compositor) {
			const char* nodeName = (const char*)payload->Data;
			LOG_INFO("Dropping Node with name: \""<< nodeName<<'\"');

			Compositor& c { *m_compositor };
			if (c.DoesNodeTypeExist(nodeName)) {
				c.AddNode(nodeName);
			}
			LOG_INFO(c.GetNodeList().size());
		}
		ImGui::EndDragDropTarget();
	}
	ImNodes::EndNodeEditor();
}

void UIWidget_Compositor::MenuBar() {
	if (!ImGui::BeginMenuBar()) return;

	ImGui::EndMenuBar();
}

void UIWidget_Compositor::LeftSideDrawerMenu() {
	
}
void UIWidget_Compositor::Update() {

}

void UIWidget_Compositor::Exit() {

}
