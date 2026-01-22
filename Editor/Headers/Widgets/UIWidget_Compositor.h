#pragma once
#include <pch.h>
#include <UI_Widget.h>
#include <arch/systems/sys_render_modules/sys_render_compositor.h>
#include <imnodes.h>

class UIWidget_Compositor : public UIWidget {
public:
	UIWidget_Compositor(std::string _widgetName);
	~UIWidget_Compositor() override;

	void Init() override;
	void Draw() override;
	void Update() override;
	void Exit() override;

private:
	Compositor* m_compositor{};
};