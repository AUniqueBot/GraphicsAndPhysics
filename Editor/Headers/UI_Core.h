#pragma once
#include <Widgets/UIWidget_Outliner.h>
#include <UI_Widget.h>

struct GLFWwindow;

class UI_Core {


public:
	void Init(unsigned _major, unsigned _minor, GLFWwindow* _window);


	void Update();

	void Exit();


	// ------------------------------------------------------------------
	void AddWidget() {};
	void RemoveWidget() {};

private:
	void DrawWidget(std::shared_ptr<UIWidget> _widget) {
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


private:
	const UIWidget_Outliner m_outliner{ "Outliner" };
};