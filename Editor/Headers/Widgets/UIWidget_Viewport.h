#pragma once
#include <UI_Widget.h>


// - declaration only ------------------
class Viewport;
class EntityID;


class UIWidget_Viewport : public UIWidget {
public:
public:
	UIWidget_Viewport(std::string _widgetName, std::shared_ptr<Viewport> _target);
	~UIWidget_Viewport() = default;


	void Init() override;
	void Draw() override;
	void Exit() override;


	void Update() override;

	void ResizeCallback() const;

	void PickObjectFromScreen() const;


private:
	std::shared_ptr<Viewport> m_viewportPointer;
};