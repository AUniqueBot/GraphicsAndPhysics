#pragma once
#include <UI_Widget.h>
#include <arch/ecs/ecs_registry.h>
#include <arch/ecs/ecs_entityFactory.h>


class UIWidget_Outliner : public UIWidget {


public:
    UIWidget_Outliner(std::string _widgetName);
    ~UIWidget_Outliner() override;

    void Init() override;
    void Draw() override;
    void Exit() override;


    void OnSelect();
    void OnUnselect();

    void Menu();


private:
    void Table();

    void SetEntityRegistry(EntityRegistry* _registry);
    void SetFactory(EntityFactory* _registry);

private:
    EntityRegistry* m_entityRegistry{};
    EntityFactory* m_entityFactory{};
};