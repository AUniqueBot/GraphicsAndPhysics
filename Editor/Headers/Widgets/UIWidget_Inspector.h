#pragma once
#include <UI_Widget.h>
#include <arch/common/component_properties.h>

class UIWidget_Inspector : public UIWidget {
public:
    UIWidget_Inspector(std::string _widgetName);


    void Init() override;
    void Draw() override;
    void Exit() override;


    void OnSelect();
    void OnUnselect();
    
private:
private:

    void DrawPropertyElement(void* object, const PropertyMD::Property& prop, const std::string& key);


    void DrawPropertyInt(void* object, const PropertyMD::Property& prop, const std::string& key);
    void DrawPropertyFloat(void* object, const PropertyMD::Property& prop, const std::string& key);
    void DrawPropertyDouble(void* object, const PropertyMD::Property& prop, const std::string& key);

    void DrawPropertyColor(void* object, const PropertyMD::Property& prop, const std::string& key);
    void DrawPropertyBoolean(void* object, const PropertyMD::Property& prop, const std::string& key);
    void DrawPropertyString(void* object, const PropertyMD::Property& prop, const std::string& key);



};


