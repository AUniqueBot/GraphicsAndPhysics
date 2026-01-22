
#include <UI_Core.h>
#include <UI_Widget.h>
#include <UI_Menu.h>

#include <iostream>
#include <sstream>

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imnodes.h>



#include <Widgets/UIWidget_Outliner.h>
#include <Widgets/UIWidget_AssetBrowser.h>
#include <Widgets/UIWidget_Viewport.h>
#include <Widgets/UIWidget_Inspector.h>
#include <Widgets/UIWidget_Compositor.h>
#include <arch/systems/sys_render.h>



void UI_Core::Init(unsigned _major, unsigned _minor, GLFWwindow* _window, Core& _core) {

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImNodes::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); 
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // optional (multi-window)
	// init 

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



	// -- widget initialisation --------------------------------
	LOG_INFO("Adding Widgets here...");
	AddWidget(std::make_shared<UIWidget_Outliner>("Outliner"));
	AddWidget(std::make_shared<UIWidget_AssetBrowser>("AssetBrowser"));
	AddWidget(std::make_shared<UIWidget_Inspector>("Inspector"));
	AddWidget(std::make_shared<UIWidget_Compositor>("Compositor"));
	// needs a viewport arg.


	

	const RenderSystem* rs = GetCore()->GetSystem<RenderSystem>();
	const auto& viewportList = rs->GetViewportManager().ViewportList();
	for (const auto& [vpId, viewport] : viewportList) {
		std::string vpName	{"Viewport"};
		vpName += "##" + vpId;
		LOG_INFO("Generated Viewport with ID"<< vpId);
		AddWidget(std::make_shared<UIWidget_Viewport>(vpName, viewport));
	}

}





void UI_Core::Update() {

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();


	ImGui::NewFrame();

	ImGuiIO& uiIO = ImGui::GetIO();


	if (GetCore()) {
		Core& c = *GetCore();
		c.GetInputRouter().RequestOwnership("UI", 60);
	}

	BeginDockSpace();
	RenderTopBar();
	RenderWidgets();


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void UI_Core::BeginDockSpace() {
	ImGuiIO& io = ImGui::GetIO();

	ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::Begin("DockSpaceRoot", nullptr, window_flags);

	ImGui::PopStyleVar(2);

	ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::End();
}


void UI_Core::Exit() {

	m_widgetStorage.clear();// remove all widgets from storage
	ImNodes::DestroyContext();
	ImGui::DestroyPlatformWindows();
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

std::string UI_Core::AddMenuItem(std::shared_ptr<UIMenu> _menu) {

	UIMenu& menu = *_menu;
	std::string menuId = menu.MenuLabel() + "##" + std::to_string(menu.MenuItemID());
	m_menuStorage.emplace(menuId, _menu);

	return std::string();
}

void UI_Core::RemoveMenuItem(std::string _id)
{
}







bool UI_Core::InputIsAllowed() const {
	return m_inputAllowed;
}

void UI_Core::InputIsAllowed(bool _setting) {
	if (m_inputAllowed == _setting) return;
	m_inputAllowed = _setting;

	UpdateAllowedInputs();
}

bool UI_Core::AllowKeyboardInput() const {
	return m_keyboardInputAllowed;
}

void UI_Core::AllowKeyboardInput(bool _setting) {
	if (m_keyboardInputAllowed != _setting) return;
	m_keyboardInputAllowed = _setting;
	UpdateAllowedInputs();
}

bool UI_Core::AllowMouseInput() const {
	return m_mouseInputAllowed;
}

void UI_Core::AllowMouseInput(bool _setting) {
	if (m_mouseInputAllowed != _setting) return;
	m_mouseInputAllowed = _setting;
	UpdateAllowedInputs();
}


















Core* UI_Core::GetCore() const {
	return m_applicationCore;
}


void UI_Core::RenderWidgets() const {
	for (const auto& [_, widget] : m_widgetStorage) {
		if (!widget) continue;
		if (!GetCore()) continue;

		widget->DrawWidget();


	}
}
void UI_Core::UpdateAllowedInputs() {
	ImGui::GetIO().WantCaptureKeyboard = m_inputAllowed && m_keyboardInputAllowed;
	ImGui::GetIO().WantCaptureMouse = m_inputAllowed && m_mouseInputAllowed;
}
void UI_Core::RenderTopBar() const {
	if (ImGui::BeginMainMenuBar()) {
		for (const auto& [_,menu] : m_menuStorage) {
			if (menu) menu->Draw();
		}
		ImGui::EndMainMenuBar();
	}

}


