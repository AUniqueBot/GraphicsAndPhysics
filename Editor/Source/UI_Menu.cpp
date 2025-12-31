#include <UI_Menu.h>
#include <imgui.h>



UI_MenuItem::UI_MenuItem(std::string _name, std::function<void()> _callback)
	: m_menuItemLabel{ _name }, m_itemFunction{_callback} {
	m_menuItemId = s_menuItemIdCounter++;
}
void UI_MenuItem::MenuLabel(const std::string& _label) {
	if (_label == m_menuItemLabel) return;
	m_menuItemLabel = _label;
}

std::string UI_MenuItem::MenuLabel() const {
	return m_menuItemLabel;
}



bool UI_MenuItem::Hovered() const {
	return m_itemFlags[0];
}

bool UI_MenuItem::Selected() const {
	return m_itemFlags[2];
}

void UI_MenuItem::Selected(bool _setting) {
	 m_itemFlags[2] = _setting;
}

bool UI_MenuItem::Disabled() const {
	return m_itemFlags[3];
}

void UI_MenuItem::Disabled(bool _setting) {
	m_itemFlags[3] = _setting;
}

unsigned UI_MenuItem::MenuItemID() const {
	return m_menuItemId;
}




void UI_MenuItem::Draw() const {
	using namespace ImGui;
	bool itemCalled = ImGui::MenuItem(m_menuItemLabel.c_str(),"", m_itemFlags[2], m_itemFlags[3]);

	
	m_itemFlags[0] = IsItemHovered();
	m_itemFlags[1] = itemCalled;


	if (itemCalled) {
		m_itemFlags[2] = true;
		if (m_itemFunction) {
			m_itemFunction();
		}
	}

}



void UIMenu::Draw() const {
	using namespace ImGui;

	if (BeginMenu) {
		for (const std::shared_ptr<UI_MenuItem>& item : m_menuItems) {
			item->Draw();
		}
		EndMenu();
	}
}

int UIMenu::AddMenuItem(UI_MenuItem _item) {
	int newIdx = m_menuItems.size();
	m_menuItems.push_back(std::make_shared<UI_MenuItem>(std::move(_item)));
	return newIdx;
}

void UIMenu::RemoveMenuItem(int _index) {
	if (_index >= m_menuItems.size()) return;
	m_menuItems.erase(m_menuItems.begin() + _index);
}


