
#include <UI_Core.h>
#include <UI_Widget.h>

#include <iostream>
#include <sstream>
//#include <imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>


#include <Widgets/UIWidget_Outliner.h>
#include <Widgets/UIWidget_AssetHandler.h>


void UI_Core::Init(unsigned _major, unsigned _minor, GLFWwindow* _window, Core& _core) {

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(_window, true);
	std::stringstream ss{};

	ss << "#version " << _major << _minor << 0 << " core";
	std::cout << ss.str() << std::endl;
	bool success = ImGui_ImplOpenGL3_Init(ss.str().c_str());
	if (!success) {
		std::cout << "BAD CALL" << std::endl;
		return;
	}
	std::cout << "GOOD CALL" << std::endl;


	// -- entity registry registration -------------------------
	m_applicationCore = &_core;
	m_entityRegistry = &_core.Registry();
	m_selectedEntity = EntityID::ENTITYID_INVALID;


	// -- widget initialisation --------------------------------
	LOG_INFO("Adding Widgets here...");
	AddWidget(std::make_shared<UIWidget_Outliner>("Outliner"));
	AddWidget(std::make_shared<UIWidget_AssetManager>("AssetManager"));

}





void UI_Core::Update() {

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Resource Manager");
	ImGui::Text("Hello from ImGui!");

	for (const auto& [_, widget]:m_widgetStorage) {
		if (widget) DrawWidget(widget);
	}


	ImGui::End();


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void UI_Core::Exit() {

	m_widgetStorage.clear();// remove all widgets from storage

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

}

std::string UI_Core::AddWidget(std::shared_ptr<UIWidget> _widget) {
	LOG_INFO("Adding a new widget...");
	if (!_widget) {
		LOG_ERROR("widget not added as there is no reference.");
		return "";
	}
	
	
	UIWidget& w {*_widget};
	w.UICore(this);
	w.Init();
	std::string widgetId = w.WidgetName() + "##" + std::to_string(w.WidgetID());
	m_widgetStorage.emplace(widgetId, _widget);

	return widgetId;
}

void UI_Core::RemoveWidget(std::string _id) {
	m_widgetStorage.erase(_id);
}



EntityID UI_Core::SelectedEntity() const {
	return m_selectedEntity;
}

void UI_Core::SelectedEntity(EntityID _id) {
	m_selectedEntity = _id;
}

Core* UI_Core::GetCore() const {
	return m_applicationCore;
}

void UI_Core::TopBar() const {

}

void UI_Core::DrawWidget(std::shared_ptr<UIWidget> _widget) {
	if (!_widget) {
		std::cerr << "no widget data\n";
		return;
	}

	const UIWidget& widget = *_widget;
	std::string ss = widget.WidgetName() + "##" + std::to_string(widget.WidgetID());
	ImGui::Begin(ss.c_str());
	widget.Draw();
	ImGui::End();
	
}
