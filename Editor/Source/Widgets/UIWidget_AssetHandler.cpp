#include <Widgets/UIWidget_AssetHandler.h>
#include <imgui.h>


UIWidget_AssetManager::UIWidget_AssetManager(std::string _widgetName) : UIWidget(_widgetName) {
};

UIWidget_AssetManager::~UIWidget_AssetManager() {

}

void UIWidget_AssetManager::Init() {
	Core& c = Core::GetInstance();

	//

	LOG_INFO("Asset Manager Init.");
}

void UIWidget_AssetManager::Draw() const {
	using namespace ImGui;



	// draw a table ish of all the assets available on the assets file path.

	// assets are found in 


}

void UIWidget_AssetManager::Exit() {

}
