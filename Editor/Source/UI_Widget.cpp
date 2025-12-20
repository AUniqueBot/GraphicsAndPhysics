#include <UI_Widget.h>

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

void UIWidget::UICore(UI_Core* _uiCore) {
    LOG_INFO("Setting UI Core...");
    m_uiCore = _uiCore;
}


