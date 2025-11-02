#pragma once
#include <string>

class UIWidget {
private:

public:
	UIWidget(std::string _widgetName) : m_widgetName{ _widgetName }, m_widgetId{ m_internalWidgetCounter } { ++m_internalWidgetCounter; }
	virtual ~UIWidget() = default;
	UIWidget(const UIWidget& _other) = delete;



	virtual void Init() {};
	virtual void Draw() const = 0;
	virtual void Exit() {};


	std::string WidgetName() const { return m_widgetName; }
	void WidgetName(std::string _newName) { if (_newName != m_widgetName) m_widgetName = _newName; }
	unsigned WidgetID() const { return m_widgetId; }



protected:
	std::string m_widgetName;
	unsigned m_widgetId;
private:
	inline static unsigned m_internalWidgetCounter = 0u;

};


