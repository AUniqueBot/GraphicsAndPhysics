#include <pch.h>
#include <arch/core/core_input.h>
#include <arch/core.h>


void InputSystem::Init(GLFWwindow* _window) {
	SetupCallbacks(_window);
}



void InputSystem::SetupCallbacks(GLFWwindow* _window) {
	m_window = _window;
	glfwSetCursorPosCallback(
		_window, [](GLFWwindow* _w, double _x, double _y) {
			static_cast<Core*>(glfwGetWindowUserPointer(_w))->GetInputSystem()._onMouseMove(_x, _y);
		}
	);


	glfwSetMouseButtonCallback(
		_window, [](GLFWwindow* _w, int _button, int _action, int _mods) {
			static_cast<Core*>(glfwGetWindowUserPointer(_w))->GetInputSystem()._onMouseButton(_button, _action, _mods);
		}
	);


	glfwSetScrollCallback(
		_window, [](GLFWwindow* _w, double _xoffset, double _yoffset) {
			static_cast<Core*>(glfwGetWindowUserPointer(_w))->GetInputSystem()._onScroll(_xoffset, _yoffset);
		}
	);



	glfwSetKeyCallback(
		_window, [](GLFWwindow* _w, int _key, int _scancode, int _action, int _mods) {
			static_cast<Core*>(glfwGetWindowUserPointer(_w))->GetInputSystem()._onKey(_key, _scancode, _action, _mods);
		}
	);

}


void InputSystem::PreUpdate() {
	m_prevMouseX = m_mouseX;
	m_prevMouseY = m_mouseY;

	m_deltaMouseX = 0;
	m_deltaMouseY = 0;

	m_scrollOffsetPrevX = m_scrollOffsetX;
	m_scrollOffsetPrevY = m_scrollOffsetY;


	m_activatedMouseButtonsPrev = m_activatedMouseButtons;
	m_activatedKeyboardButtonsPrev = m_activatedKeyboardButtons;

}

void InputSystem::Update() {
	
}

void InputSystem::PostUpdate() {
	if (m_freezeMousePosition) {
		MoveMouse(m_prevMouseX, m_prevMouseY);
	}
}

// - query -----------------------------------------


bool InputSystem::IsKeyPressed_Internal(INPUT_KEY _key) const {
	if (_key == _KEY_COUNT || _key == _KEY_UNKNOWN) return false;
	return m_activatedKeyboardButtons[_key] && !m_activatedKeyboardButtonsPrev[_key];
}

bool InputSystem::IsKeyHeld_Internal(INPUT_KEY _key) const {
	if (_key == _KEY_COUNT || _key == _KEY_UNKNOWN) return false;
	return m_activatedKeyboardButtons[_key] && m_activatedKeyboardButtonsPrev[_key];
}

bool InputSystem::IsKeyReleased_Internal(INPUT_KEY _key) const {
	if (_key == _KEY_COUNT || _key == _KEY_UNKNOWN) return false;
	return !m_activatedKeyboardButtons[_key] && m_activatedKeyboardButtonsPrev[_key];
}


bool InputSystem::IsMouseButtonClicked_Internal(INPUT_MOUSE_BUTTON _button) const {
	if (_button == _MOUSE_COUNT || _button == _MOUSE_UNKNOWN) return false;
	return m_activatedMouseButtons[_button] && !m_activatedMouseButtonsPrev[_button];
}

bool InputSystem::IsMouseButtonHeld_Internal(INPUT_MOUSE_BUTTON _button) const {
	if (_button == _MOUSE_COUNT || _button == _MOUSE_UNKNOWN) return false;
	return m_activatedMouseButtons[_button] && m_activatedMouseButtonsPrev[_button];
}

bool InputSystem::IsMouseButtonReleased_Internal(INPUT_MOUSE_BUTTON _button) const {
	if (_button == _MOUSE_COUNT || _button == _MOUSE_UNKNOWN) return false;
	return !m_activatedMouseButtons[_button] && m_activatedMouseButtonsPrev[_button];
}

bool InputSystem::IsMouseButtonClicked(INPUT_MOUSE_BUTTON _button) const {
	return IsMouseButtonClicked_Internal(_button) && m_allowInputs && m_allowMouse;
}

bool InputSystem::IsMouseButtonHeld(INPUT_MOUSE_BUTTON _button) const {
	return IsMouseButtonHeld_Internal(_button) && m_allowInputs && m_allowMouse;
}

bool InputSystem::IsMouseButtonReleased(INPUT_MOUSE_BUTTON _button) const {
	return IsMouseButtonReleased_Internal(_button) && m_allowInputs && m_allowMouse;
}

double InputSystem::ScrollXOffset() const {
	return m_scrollOffsetX - m_scrollOffsetPrevX;
}

double InputSystem::ScrollYOffset() const {
	return m_scrollOffsetY - m_scrollOffsetPrevY;
}



bool InputSystem::IsKeyPressed(INPUT_KEY _key) const {
	// needs filter 
	return IsKeyPressed_Internal(_key) && (m_allowInputs && m_allowKeyboard);
}

bool InputSystem::IsKeyHeld(INPUT_KEY _key) const {
	return IsKeyHeld_Internal(_key) && (m_allowInputs && m_allowKeyboard);
}

bool InputSystem::IsKeyReleased(INPUT_KEY _key) const {
	return IsKeyReleased_Internal(_key) && (m_allowInputs && m_allowKeyboard);
}



glm::vec2 InputSystem::GetMousePosition() const {
	return (m_allowInputs && m_allowMouse) ? GetMouseDelta_Internal() : glm::vec2{};
}

glm::vec2 InputSystem::GetMouseDelta() const {
	return (m_allowInputs && m_allowMouse) ? GetMouseDelta_Internal() : glm::vec2{};
}

bool& InputSystem::CursorPositionFrozen() {
	return m_freezeMousePosition;
}

const bool& InputSystem::CursorPositionFrozen() const {
	return m_freezeMousePosition;
}

void InputSystem::CursorPositionFrozen(bool _setting) {
	m_freezeMousePosition = _setting;
}

bool InputSystem::DisplayMouse() const {
	return m_mouseVisible;
}

void InputSystem::DisplayMouse(bool _setting) {
	m_mouseVisible = _setting;
	glfwSetInputMode(m_window, GLFW_CURSOR, m_mouseVisible ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void InputSystem::MoveMouse(glm::vec2 _pos) {
	m_prevMouseX = m_mouseX;
	m_prevMouseY = m_mouseY;
	m_mouseX = _pos.x;
	m_mouseY = _pos.y;

	if (m_window) {
		glfwSetCursorPos(m_window, m_mouseX, m_mouseY);
	}
}

void InputSystem::MoveMouse(float _x, float _y) {
	MoveMouse({_x, _y});
}

glm::vec2 InputSystem::GetMousePosition_Internal() const {
	return glm::vec2(m_mouseX, m_mouseY);
}

glm::vec2 InputSystem::GetMouseDelta_Internal() const {
	return glm::vec2{ m_deltaMouseX, m_deltaMouseY };
}

void InputSystem::ClearInputs() {
	m_activatedKeyboardButtons = false;
	m_activatedKeyboardButtonsPrev = false;
	m_activatedMouseButtons = false;
	m_activatedMouseButtonsPrev = false;
}



bool InputSystem::InputIsAllowed() const {
	return m_allowInputs;
}

void InputSystem::InputIsAllowed(bool _setting) {
	m_allowInputs = _setting;
}

bool InputSystem::AllowKeyboardInput() const {
	return m_allowKeyboard;
}

void InputSystem::AllowKeyboardInput(bool _setting) {
	m_allowKeyboard = _setting;
}

bool InputSystem::AllowMouseInput() const {
	return m_allowMouse;
}

void InputSystem::AllowMouseInput(bool _setting) {
	m_allowMouse = _setting;
}



// - callbacks -------------------------------------

void InputSystem::_onMouseMove(double _xpos, double _ypos) {

	m_mouseX = _xpos;
	m_mouseY = _ypos;
	m_deltaMouseX = m_mouseX - m_prevMouseX;
	m_deltaMouseY = m_mouseY - m_prevMouseY;
}

void InputSystem::_onMouseButton(int _button, int _action, int _mods) {
	//if (m_disableInputs) return;

	if (_button < 0 || _button > GLFW_MOUSE_BUTTON_MIDDLE) return;

	if (!GLFW_TO_MOUSE_BUTTON_MAP.contains(_button)) return;

	INPUT_MOUSE_BUTTON currentKey = GLFW_TO_MOUSE_BUTTON_MAP.at(_button);

	m_activatedMouseButtons[currentKey] =
		_action == GLFW_PRESS ? true :
		_action == GLFW_RELEASE ? false :
		m_activatedMouseButtons[currentKey];
}

void InputSystem::_onScroll(double _xoffset, double _yoffset) {
	//if (m_disableInputs) return;

	m_scrollOffsetX += _xoffset;
	m_scrollOffsetY += _yoffset;
}

void InputSystem::_onKey(int _key, int _scancode, int _action, int _mods) {
	//if (m_disableInputs) return;

	if (_key < 0 || _key > GLFW_KEY_LAST) return;
	if (!GLFW_TO_KEY_MAP.contains(_key)) return;
	INPUT_KEY currentKey = GLFW_TO_KEY_MAP.at(_key);
	m_activatedKeyboardButtons[currentKey] =
		_action == GLFW_PRESS ? true :
		_action == GLFW_RELEASE ? false :
		m_activatedKeyboardButtons[currentKey];
}
