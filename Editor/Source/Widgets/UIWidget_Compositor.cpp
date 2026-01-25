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
		
	const ImGuiTableFlags tableFlags = 
		ImGuiTableFlags_::ImGuiTableFlags_Resizable | 
		ImGuiTableFlags_::ImGuiTableFlags_Sortable |
		ImGuiTableFlags_::ImGuiTableFlags_Reorderable
		;
		if (ImGui::BeginTable("Node Table", 2, tableFlags)) {
			ImGui::TableSetupColumn("Name");
			ImGui::TableSetupColumn("Category");
			ImGui::TableHeadersRow();
			ImGui::TableSetColumnIndex(0);

			for (const CompositionNodeMetadata& data : list) {
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGuiSelectableFlags selectableFlags =
					ImGuiSelectableFlags_::ImGuiSelectableFlags_AllowDoubleClick |
					ImGuiSelectableFlags_::ImGuiSelectableFlags_SpanAllColumns
					;
				ImGui::Selectable(data.m_nodeName.c_str(), false, selectableFlags);
				if (ImGui::BeginDragDropSource()) {
					ImGui::SetDragDropPayload("CompositorNodeMetadata", data.m_nodeName.c_str(), data.m_nodeName.size() + 1);
					ImGui::Text(data.m_nodeName.c_str());
					ImGui::EndDragDropSource();
				}
				ImGui::TableSetColumnIndex(1);
				ImGui::Text(data.m_category.c_str());
			}
			ImGui::EndTable();
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();
	CompositorGrid();
	 
}

void UIWidget_Compositor::CompositorGrid() {
	ImNodes::BeginNodeEditor();

	ImGui::Dummy(ImGui::GetContentRegionAvail());
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CompositorNodeMetadata")) {
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
	DrawNodes(); 
	ImNodes::EndNodeEditor();

}

void UIWidget_Compositor::DrawNodes() {
	Compositor& c{ *m_compositor };
	for (auto& node : c.GetNodeList()) {
		NodeID nid{ node.GetID() };
		ImNodes::BeginNode(node.GetID());

		ImNodes::BeginNodeTitleBar();
		ImGui::Text(node.Name().c_str());
		ImNodes::EndNodeTitleBar();
		
		for (auto& input : node.GetInputs()) {
			SlotID id{ CompositionNode::GetUniquePinID(nid, input.ID(), false) };
			ImNodes::BeginInputAttribute(id);
			ImGui::Text(input.Name().c_str());
			ImNodes::EndInputAttribute();
		}

		for (auto& output : node.GetOutputs()) {
			SlotID id{ CompositionNode::GetUniquePinID(nid, output.ID(), true) };
			ImNodes::BeginOutputAttribute(id );
			ImGui::Text(output.Name().c_str());
			ImNodes::EndOutputAttribute();
		}



		ImNodes::EndNode();

	}
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
