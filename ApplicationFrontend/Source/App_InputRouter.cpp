#include "App_InputRouter.h"
#pragma once

bool InputRouter::GameInputEnabled() const {
    return m_core.GetInputSystem().InputIsAllowed();
}

void InputRouter::GameInputEnabled(bool _setting) {
    m_core.GetInputSystem().InputIsAllowed(_setting);
}

bool InputRouter::UIInputEnabled() const {
    return false;
}

void InputRouter::UIInputEnabled(bool _setting)
{
}
