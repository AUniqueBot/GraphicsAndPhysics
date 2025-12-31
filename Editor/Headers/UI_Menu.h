#pragma once
#include <pch.h>
#include <functional>
// TODO - Create a ui menu item.

class UI_MenuItem {
public:
	UI_MenuItem(std::string _name, std::function<void()> _callback = nullptr);
	virtual void Draw() const;


	void MenuLabel(const std::string& _label);
	std::string MenuLabel() const;

	bool Hovered() const;
	bool Selected() const;
	void Selected(bool _setting);
	bool Disabled() const;
	void Disabled(bool _setting);

	unsigned MenuItemID() const;

private:
	/*
		0 = hovered over,
		1 = clicked,
		2 = selected,
		3 = disabled,
	*/

	inline static unsigned s_menuItemIdCounter	{};
	unsigned m_menuItemId;
	mutable std::bitset<4> m_itemFlags;
	std::string m_menuItemLabel{};
	std::function<void()> m_itemFunction;
};


class UIMenu : public UI_MenuItem {

public:
	void Draw() const override;
	int AddMenuItem(UI_MenuItem _item);
	void RemoveMenuItem(int _index);
private:
	std::vector<std::shared_ptr<UI_MenuItem>> m_menuItems;
};


