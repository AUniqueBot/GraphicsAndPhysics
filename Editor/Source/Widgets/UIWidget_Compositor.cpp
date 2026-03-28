#include <Widgets/UIWidget_Compositor.h>
#include <arch/systems/sys_render.h>
#include <imnodes.h>

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
	LeftSideDrawerMenu();

	
	ImGui::SameLine();
	CompositorGrid();
	 
}

void UIWidget_Compositor::CompositorGrid() {
	ImNodes::BeginNodeEditor();
	ImNodes::PushStyleVar(ImNodesStyleVar_::ImNodesStyleVar_LinkLineSegmentsPerLength, 1);
	ImGui::Dummy(ImGui::GetContentRegionAvail());
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CompositorNodeMetadata")) {
			const char* nodeName = (const char*)payload->Data;
			LOG_INFO("Dropping Node with name: \""<< nodeName<<'\"');

			Compositor& c { *m_compositor };
			if (c.DoesNodeTypeExist(nodeName)) {
				NodeID id = c.AddNode(nodeName);
				auto nodeView{ c.GetNode(id) };
				if (nodeView) {
					ImVec2 mouseScreenPos = ImGui::GetMousePos();
					ImNodes::SetNodeScreenSpacePos(id, mouseScreenPos);
				}
			}
			LOG_INFO(c.GetNodeList().size());
		}
		ImGui::EndDragDropTarget();
	}
	
	
	DrawNodes(); 
	DrawLinks();
	ImNodes::EndNodeEditor();

}

void UIWidget_Compositor::DrawNodes() {
	Compositor& c{ *m_compositor };
	for (auto& node : c.GetNodeList()) {
		NodeID nid{ node.GetID() };
		ImNodes::BeginNode(nid);
		
		//ImNodes::SetNodeGridSpacePos(nid, { node.Position().x , node.Position().y });
		ImNodes::BeginNodeTitleBar();
		ImGui::Text(node.Name().c_str());
		ImGui::SameLine();
		ImGui::Text("(%i)", nid);
		ImNodes::EndNodeTitleBar();
		
		for (auto& input : node.GetInputs()) {
			ImNodes::BeginInputAttribute(input.GetPinID());
			ImNodes::PushAttributeFlag(ImNodesAttributeFlags_::ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
			ImGui::Text(input.Name().c_str());
			ImNodes::EndInputAttribute();
		}

		for (auto& output : node.GetOutputs()) {
			ImNodes::BeginOutputAttribute(output.GetPinID());
			ImGui::Text(output.Name().c_str());
			ImNodes::ClearLinkSelection(); 
			ImNodes::EndOutputAttribute();
		}

		ImNodes::EndNode();

	}
}

void UIWidget_Compositor::DrawLinks() {
	Compositor& c	{ *m_compositor };
	auto links = c.GetLinkList();

	for (auto& link : links) {
		ImNodes::Link(link.GetLinkID(), link.m_fromPin, link.m_toPin);
	}

}

void UIWidget_Compositor::MenuBar() {
	if (!ImGui::BeginMenuBar()) return;

	ImGui::EndMenuBar();
}

void UIWidget_Compositor::LeftSideDrawerMenu() {
	float totalWidth{ ImGui::GetContentRegionAvail().x };
	static float menuWidth{ 240.f };
	constexpr float minMenuWidth{ 120.f };
	menuWidth = glm::clamp(
		menuWidth,
		minMenuWidth,
		totalWidth - minMenuWidth - 4.f
	);

	Compositor& comp{ *m_compositor };

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
}
void UIWidget_Compositor::UpdateLinks() {
	Compositor& compositor { *m_compositor };
	PinID start{};
	PinID end{};
	if (ImNodes::IsLinkCreated(&start, &end)) {
		LOG_INFO("A Link has been created between " << start << " & " << end);

		LinkID linkId = compositor.AddLink(start, end);
	}
	start = {};


	LinkID link{};
	if (ImNodes::IsLinkDestroyed(&link)) {
		LOG_INFO("Killing Link.");
		compositor.RemoveLink(link);
	}
}
void UIWidget_Compositor::Update() {
	UpdateLinks();
}

void UIWidget_Compositor::Exit() {

}
