#pragma once
#include <UI_Widget.h>

class UIWidget_AssetManager : public UIWidget {
public:
	UIWidget_AssetManager(std::string _widgetName);
	~UIWidget_AssetManager();


	void Init() override;
	void Draw() const override;
	void Exit() override;


};
 
