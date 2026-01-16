#pragma once
#include <pch.h>
#include <functional>
#include <arch/core.h>
#include <UI_Core.h>

struct InputFlags {
	bool m_mouseInputAllowed;
	bool m_keyboardInputAllowed;
};



class InputRouter {



public:
	// we need core and ui core handlers

	InputRouter(Core& _c, UI_Core& _uic) : m_core{ _c }, m_uiCore{_uic} {};
	
	bool GameInputEnabled() const;
	void GameInputEnabled(bool _setting);

	
	bool UIInputEnabled() const;
	void UIInputEnabled(bool _setting);



private:


private:
	Core& m_core;
	UI_Core& m_uiCore;

};