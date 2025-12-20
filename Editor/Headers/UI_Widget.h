#pragma once
#include <string>
#include <UI_Core.h>



class UIWidget {
private:
	friend class UI_Core;
public:
	UIWidget(std::string _widgetName);
	virtual ~UIWidget() = default;
	UIWidget(const UIWidget& _other) = delete;



	virtual void Init() {};
	virtual void Draw() const = 0;
	virtual void Exit() {};


	std::string WidgetName() const;
	void WidgetName(std::string _newName);
	unsigned WidgetID() const;


	// -- UI Core -------------------------------------------
	UI_Core* UICore();
	const UI_Core* UICore() const;

	Core* ApplicationCore();
	const Core* ApplicationCore() const;

protected:
	void UICore(UI_Core* _uiCore);
	UI_Core* m_uiCore								{};


	std::string m_widgetName;
	unsigned m_widgetId;
private:
	inline static unsigned m_internalWidgetCounter	{ 0u };
};


