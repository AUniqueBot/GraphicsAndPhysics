#pragma once
#include <pch.h>
#include <imnodes.h>
#include <UI_Widget.h>
#include <arch/systems/sys_render_modules/sys_render_compositor.h>
#include <arch/systems/sys_render_modules/sys_render_viewportManager.h>



class UIWidget_Compositor : public UIWidget {
public:
	UIWidget_Compositor(std::string _widgetName);
	~UIWidget_Compositor() override;

	void Init() override;
	void Draw() override;
	void Update() override;
	void Exit() override;

public:
	void CompositorGrid();
	void MenuBar();
	void LeftSideDrawerMenu();
public:
	void EvaluateNodes(); // call the compositor to evaluate.



private:
	Compositor* m_compositor					 {};
	ViewportManager* m_viewportManager			{};
	Viewport::ViewportID m_selectedViewport		{};
};


