#pragma once
#include <UI_Widget.h>
#include <unordered_map>
#include <memory>

struct GLFWwindow;

class UI_Core {


public:
	void Init(unsigned _major, unsigned _minor, GLFWwindow* _window);


	void Update();

	void Exit();


	// ------------------------------------------------------------------
	std::string AddWidget(std::shared_ptr<UIWidget> _widget);
	void RemoveWidget(std::string _id);

private:
	void DrawWidget(std::shared_ptr<UIWidget> _widget);


private:
	std::unordered_map<std::string, std::shared_ptr<UIWidget>> m_widgetStorage;
};