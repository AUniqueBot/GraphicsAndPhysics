
#include <UI_Core.h>
#include <iostream>
#include <sstream>
//#include <imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>


#include <Widgets/UIWidget_Outliner.h>


void UI_Core::Init(unsigned _major, unsigned _minor, GLFWwindow* _window) {

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
	bool success = ImGui_ImplOpenGL3_Init("#version 460 core");
	if (success) {
		std::cout << "GOOD CALL" << std::endl;
		return;
	}
	std::cout << "BAD CALL" << std::endl;


	// init windows here.
	

}





void UI_Core::Update() {

	

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Resource Manager");
	ImGui::Text("Hello from ImGui!");
	ImGui::End();


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void UI_Core::Exit() {

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

}

std::string UI_Core::AddWidget(std::shared_ptr<UIWidget> _widget) {
	if (!_widget) return "";
	const UIWidget& w {*_widget};
	std::string widgetId = w.WidgetName() + "##" + std::to_string(w.WidgetID());

	m_widgetStorage.emplace(widgetId, _widget);

}

void UI_Core::RemoveWidget(std::string _id) {
	m_widgetStorage.erase(_id);
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
