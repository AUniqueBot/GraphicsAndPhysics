#pragma once
#include <UI_Widget.h>


class UIWidget_Inspector : public UIWidget {
public:
    UIWidget_Inspector(std::string _widgetName);


    void Init() override;
    void Draw() override;
    void Exit() override;


    void OnSelect();
    void OnUnselect();
    
private:

    void RenderComponents() {

    }



    // selected object.
};
