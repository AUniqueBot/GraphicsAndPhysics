
#pragma once
#include <UI_Widget.h>


class UIWidget_Outliner : public UIWidget {


public:
	UIWidget_Outliner(std::string _widgetName);
	~UIWidget_Outliner() override;

	void Init() const override;
	void Draw() const override;
	void Exit() const override;

	void OnSelect();

private:
	

};