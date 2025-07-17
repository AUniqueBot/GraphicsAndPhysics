#include <pch.h>
#include <arch/core/core_input.h>
#include <arch/core.h>


void InputSystem::Init(GLFWwindow* _window) {
	SetupCallbacks(_window);
}



void InputSystem::SetupCallbacks(GLFWwindow* _window) {
	
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

void InputSystem::Update() {

	// update bitsets
	m_activatedKeyboardButtonsPrev = m_activatedKeyboardButtons;
	m_activatedMouseButtons = m_activatedMouseButtonsPrev;
}


// - query -----------------------------------------


bool InputSystem::IsKeyPressed(INPUT_KEY _key) const {
	if (_key == _KEY_COUNT || _key == _KEY_UNKNOWN) return false;
	return m_activatedKeyboardButtons[_key] && !m_activatedKeyboardButtonsPrev[_key];
}

bool InputSystem::IsKeyHeld(INPUT_KEY _key) const {
	if (_key == _KEY_COUNT || _key == _KEY_UNKNOWN) return false;
	return m_activatedKeyboardButtons[_key] && m_activatedKeyboardButtonsPrev[_key];
}

bool InputSystem::IsKeyReleased(INPUT_KEY _key) const {
	if (_key == _KEY_COUNT || _key == _KEY_UNKNOWN) return false;
	return !m_activatedKeyboardButtons[_key] && m_activatedKeyboardButtonsPrev[_key];
}

bool InputSystem::IsMouseButtonClicked(INPUT_MOUSE_BUTTON _button) const {
	if (_button == _MOUSE_COUNT || _button == _MOUSE_UNKNOWN) return false;
	return m_activatedMouseButtons[_button] && !m_activatedMouseButtonsPrev[_button];
}

bool InputSystem::IsMouseButtonHeld(INPUT_MOUSE_BUTTON _button) const {
	if (_button == _MOUSE_COUNT || _button == _MOUSE_UNKNOWN) return false;
	return m_activatedMouseButtons[_button] && m_activatedMouseButtonsPrev[_button];
}

bool InputSystem::IsMouseButtonReleased(INPUT_MOUSE_BUTTON _button) const {
	if (_button == _MOUSE_COUNT || _button == _MOUSE_UNKNOWN) return false;
	return !m_activatedMouseButtons[_button] && m_activatedMouseButtonsPrev[_button];
}

glm::vec2 InputSystem::GetMousePosition() const {
	return glm::vec2(m_mouseX, m_mouseY);
}

glm::vec2 InputSystem::GetMouseDelta() const {
	static glm::vec2 prevMousePos	{ m_mouseX, m_mouseY };
	const glm::vec2 currentMousePos	{ GetMousePosition() };
	const glm::vec2 mouseDelta{ currentMousePos - prevMousePos };
	prevMousePos = currentMousePos;
	return mouseDelta;
}



// - callbacks -------------------------------------

void InputSystem::_onMouseMove(double _xpos, double _ypos) {
	m_mouseX = _xpos;
	m_mouseY = _ypos;
}

void InputSystem::_onMouseButton(int _button, int _action, int _mods) {
	if (_button < 0 || _button > GLFW_MOUSE_BUTTON_MIDDLE) return;

	if (!GLFW_TO_MOUSE_BUTTON_MAP.contains(_button)) return;

	INPUT_MOUSE_BUTTON currentKey = GLFW_TO_MOUSE_BUTTON_MAP.at(_button);
	
	m_activatedMouseButtons[currentKey] =
		_action == GLFW_PRESS ? true :
		_action == GLFW_RELEASE ? false :
		m_activatedMouseButtons[currentKey];
}

void InputSystem::_onScroll(double _xoffset, double _yoffset) {
	m_scrollOffsetX += _xoffset;
	m_scrollOffsetY += _yoffset;
}

void InputSystem::_onKey(int _key, int _scancode, int _action, int _mods) {

	if (_key < 0 || _key > GLFW_KEY_LAST) return;
	if (!GLFW_TO_KEY_MAP.contains(_key)) return;
	
	INPUT_KEY currentKey = GLFW_TO_KEY_MAP.at(_key);
	m_activatedKeyboardButtons[currentKey] =
		_action == GLFW_PRESS ? true :
		_action == GLFW_RELEASE ? false :
		m_activatedKeyboardButtons[currentKey];
}
