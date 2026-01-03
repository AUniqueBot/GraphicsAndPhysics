#pragma once
#include <UI_Widget.h>

class Viewport;

class UIWidget_Viewport : public UIWidget {
public:
public:
	UIWidget_Viewport(std::string _widgetName, std::shared_ptr<Viewport> _target);
	~UIWidget_Viewport() = default;


	void Init() override;
	void Draw() override;
	void Exit() override;

	//

	void ResizeCallback() const;


private:
	std::shared_ptr<Viewport> m_viewportPointer;
};