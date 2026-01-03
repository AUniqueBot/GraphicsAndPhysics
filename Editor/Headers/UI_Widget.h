#pragma once
#include <string>
#include <UI_Core.h>
//#include <imgui.h>



class UIWidget {
private:
	friend class UI_Core;
public:
	UIWidget(std::string _widgetName);
	virtual ~UIWidget() = default;
	UIWidget(const UIWidget& _other) = delete;



	virtual void Init() {};
	virtual void Draw() const = 0;
	virtual void Update(); 
	virtual void Exit() {};



	std::string WidgetName() const;
	void WidgetName(std::string _newName);
	unsigned WidgetID() const;


	// -- flags ---------------------------------------------
	bool WidgetIsHoveredOver() const; 
	bool WidgetIsFocused() const; 
	bool WidgetIsVisible() const;
	bool WidgetIsCollapsed() const;

	// -- UI Core -------------------------------------------
	UI_Core* UICore();
	const UI_Core* UICore() const;

	Core* ApplicationCore();
	const Core* ApplicationCore() const;


	void EngineInputDisabled(bool _setting);
	bool EngineInputDisabled() const;


protected:
	// - drag and drop ---------------------------------------
	
	//// wrapper functions
	//template <typename T>
	//void DragAndDropSource(
	//	std::string _dragSourceIdentifier, 
	//	std::string _itemName, 
	//	const T& _value, 
	//	ImGuiCond _conditions = 0
	//) {
	//	static_assert(std::is_trivially_copyable_v<T>,
	//		"Drag and Drop type must be trivially copyable");
	//	ImGui::SetDragDropPayload(_dragSourceIdentifier.c_str(), _value, sizeof(T), _conditions);
	//}
	//

	//
	//template <typename T>
	//void DragAndDropTarget(
	//	std::string _dragSourceIdentifier,
	//	T& _value,
	//	ImGuiCond _conditions = 0
	//) const {
	//	ImGui::AcceptDragDropPayload(_dragSourceIdentifier.c_str(), _conditions);
	//}


protected:
	void UICore(UI_Core* _uiCore);
	void DrawWidget() const; 

protected:
	UI_Core* m_uiCore								{};
	std::string m_widgetName;
	unsigned m_widgetId;


	/* 
		0 = hovered over, 
		1 = focused, 
		2 = visible,
		3 = collapsed
	*/
	mutable std::bitset<4> m_widgetBitset			{ 0 };


private:
	inline static unsigned m_internalWidgetCounter	{ 0u };
};


