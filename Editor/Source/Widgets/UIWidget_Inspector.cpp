#include <Widgets/UIWidget_Inspector.h>
#include <imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include <UI_Core.h>

UIWidget_Inspector::UIWidget_Inspector(std::string _widgetName) : UIWidget(_widgetName) {

}

void UIWidget_Inspector::Draw() const {
	using namespace ImGui;
	const UI_Core* puic = UICore();
	const Core* papc = ApplicationCore();
	
	if (!puic || !papc) return;


	const UI_Core& uic = *puic;
	const Core& core = *papc;

	const EntityID selectedID = uic.SelectedEntity();
	
	

	std::string s;
	InputText("Object Name", &s);
	
	
}
