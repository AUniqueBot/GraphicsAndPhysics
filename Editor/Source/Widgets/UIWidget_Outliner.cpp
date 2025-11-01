#include <Widgets/UIWidget_Outliner.h>
#include <imgui.h>



UIWidget_Outliner::UIWidget_Outliner(std::string _widgetName) : UIWidget(_widgetName) {
	
}

UIWidget_Outliner::~UIWidget_Outliner() {
}

void UIWidget_Outliner::Init() const {

}

void UIWidget_Outliner::Draw() const {
	using namespace ImGui;
	
		// draw contents
	Text("Outliner Here");
	std::string tableName= WidgetName() + "_list_" + "##" + std::to_string(WidgetID());
	ImGui::BeginTable(tableName.c_str(), 4);

	// reference to scene manager required



}

void UIWidget_Outliner::Exit() const {

}

void UIWidget_Outliner::OnSelect() {

}


