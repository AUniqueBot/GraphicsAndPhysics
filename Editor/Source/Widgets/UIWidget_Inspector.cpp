#include <Widgets/UIWidget_Inspector.h>
#include <imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include <UI_Core.h>

UIWidget_Inspector::UIWidget_Inspector(std::string _widgetName) : UIWidget(_widgetName) {

}



void UIWidget_Inspector::Init() {

}


void UIWidget_Inspector::Draw() {
	using namespace ImGui;
	UI_Core* puic = UICore();
	Core* papc = ApplicationCore();
	
	if (!puic || !papc) return;


	UI_Core& uic = *puic;
	Core& core = *papc;

	EntityID selectedID = uic.SelectedEntity();
	EntityView selectedObj = core.Registry().Get(selectedID);
	if (!selectedObj) {
		Text("No object selected with ID [%i]", selectedID);
		return;
	}



	Entity& obj = *selectedObj;
	std::string s{ selectedObj->Name() };
	if (InputText("Object Name", &s) && ImGui::IsKeyPressed(ImGuiKey_Enter)) {
		obj.Name(s);
	}
	
	

	// render components here.

	

	
}


void UIWidget_Inspector::Exit() {

}

