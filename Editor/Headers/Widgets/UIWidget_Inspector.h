#pragma once
#include <UI_Widget.h>


class UIWidget_Inspector : public UIWidget {
public:
    UIWidget_Inspector(std::string _widgetName);
    ~UIWidget_Inspector() override;

    void Init() override;
    void Draw() const override;
    void Exit() override;


    void OnSelect();
    void OnUnselect();

private:

    // selected object.
};
