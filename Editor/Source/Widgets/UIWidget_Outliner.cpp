#include <Widgets/UIWidget_Outliner.h>
#include <arch/core.h>
#include <imgui.h>



UIWidget_Outliner::UIWidget_Outliner(std::string _widgetName) : UIWidget(_widgetName) {
	
}

UIWidget_Outliner::~UIWidget_Outliner() {
}

void UIWidget_Outliner::Init() {
	Core& c = Core::GetInstance(); // only on init.
	SetEntityRegistry(&c.Registry()); // setup aliases
	std::cout << "Initialisation Complete!\n";
}

void UIWidget_Outliner::Draw() const {
	using namespace ImGui;
	std::cout << "I'm being called here" << std::endl;



	if (!m_entityRegistry) return;
	
	// draw contents
	// reference to scene manager required
	bool expanded = CollapsingHeader("Something");
	
	if (!expanded) {
		return;
	}

	SeparatorText("// -- Outliner -- //");
	std::string tableName = WidgetName() + "_list_" + "##" + std::to_string(WidgetID());
	bool tableCreated = ImGui::BeginTable(tableName.c_str(), 4);
	if (!tableCreated) {
		return;
	}

	// -- aliases -------------------------
	EntityRegistry& registry = *m_entityRegistry;
	



	ImGui::EndTable();

}

void UIWidget_Outliner::Exit() {
	// -- cleanup -------------------------
	m_entityRegistry = nullptr;
}

void UIWidget_Outliner::OnSelect() {
	
}

void UIWidget_Outliner::OnUnselect() {

}

void UIWidget_Outliner::SetEntityRegistry(EntityRegistry* _registry) {
	m_entityRegistry = _registry;
	LOG_INFO("Registry updated!");
}


