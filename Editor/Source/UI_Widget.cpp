#include <UI_Widget.h>
#include <imgui.h>

UIWidget::UIWidget(std::string _widgetName) : 
    m_widgetName{ _widgetName }, 
    m_widgetId{ m_internalWidgetCounter } {
    ++m_internalWidgetCounter;

}

std::string UIWidget::WidgetName() const {
    return m_widgetName; 
}
void UIWidget::WidgetName(std::string _newName) {
    if (_newName != m_widgetName) m_widgetName = _newName; 
}


unsigned UIWidget::WidgetID() const {
    return m_widgetId; 
}

void UIWidget::Update() {
}


// -- flags -----------------------------------------
bool UIWidget::WidgetIsHoveredOver() const{
    return m_widgetBitset[0];
}
bool UIWidget::WidgetIsFocused() const {
    return m_widgetBitset[1];
}
bool UIWidget::WidgetIsVisible() const {
    return m_widgetBitset[2];
}
bool UIWidget::WidgetIsCollapsed() const {
    return m_widgetBitset[3];
}



UI_Core* UIWidget::UICore() {
    return m_uiCore;
}
const UI_Core* UIWidget::UICore() const {
    return m_uiCore;
}

Core* UIWidget::ApplicationCore() {
    return m_uiCore ? m_uiCore->GetCore() : nullptr;
}

const Core* UIWidget::ApplicationCore() const {
    return m_uiCore ? m_uiCore->GetCore() : nullptr;
}

void UIWidget::EngineInputDisabled(bool _setting) {
    if (!m_uiCore) return;
    Core* c = m_uiCore->GetCore();
    if (!c) return;
    c->GetInputSystem().InputIsDisabled(_setting);

}
bool UIWidget::EngineInputDisabled() const {
    if (!m_uiCore) return false;
    Core* c = m_uiCore->GetCore();
    if (!c) return false;
    return c->GetInputSystem().InputIsDisabled();
}



void UIWidget::UICore(UI_Core* _uiCore) {
    LOG_INFO("Setting UI Core...");
    m_uiCore = _uiCore;
}

void UIWidget::DrawWidget() const {
    std::string ss = WidgetName() + "##" + std::to_string(WidgetID());
    ImGui::Begin(ss.c_str());

    Draw();
    
    // - adjust flags ---------------------------
    m_widgetBitset[0] = ImGui::IsWindowHovered();
    m_widgetBitset[1] = ImGui::IsWindowFocused();
    m_widgetBitset[2] = ImGui::IsWindowAppearing();
    m_widgetBitset[3] = ImGui::IsWindowCollapsed();
    
    
    ImGui::End();
}


