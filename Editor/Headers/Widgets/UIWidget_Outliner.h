#pragma once
#include <UI_Widget.h>
#include <arch/ecs/ecs_registry.h>


class UIWidget_Outliner : public UIWidget {


public:
    UIWidget_Outliner(std::string _widgetName);
    ~UIWidget_Outliner() override;

    void Init() override;
    void Draw() override;
    void Exit() override;


    void OnSelect();
    void OnUnselect();


    void SetEntityRegistry(EntityRegistry* _registry);

private:
    void Table();


private:
    EntityRegistry* m_entityRegistry{};

};