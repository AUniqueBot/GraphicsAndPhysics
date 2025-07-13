#include <pch.h>
#include <arch/core/core_viewport.h>
#include <arch/core.h>


// - general ----------------------------------------------------------

void Viewport::Init() {

}

void Viewport::Start() {

}

void Viewport::Render() {
	
}




void Viewport::Update() {
	UpdateAttributes();
}



// - matrix -----------------------------------------------------------

void Viewport::UpdateAttributes() {
	
	
	if (m_transformDirty) {
		glm::mat4 pos = glm::translate(
			glm::mat4{ 1.f },
			m_position
		);
		glm::mat4 rot{ m_rotation };
		m_viewportMatrix = pos * rot;

		m_transformDirty = false;
	}


	if (m_projectionDirty) {
		if (m_isOrthographic) {
			// do nothing for now
		}
		else {
			m_projectionMatrix = glm::perspective(
				glm::radians(m_verticalFOV),
				m_aspectRatio,
				m_nearClip,
				m_farClip
			);
		}

		m_projectionDirty = false;
	}
}



const glm::mat4& Viewport::CameraMatrix() const {
	return m_viewportMatrix;
}

const glm::mat4& Viewport::ProjectionMatrix() const {
	return m_projectionMatrix;
}


// - viewport management ----------------------------------------------

void Viewport::ViewportDimensions(const glm::ivec2& _dimensions) {
	m_viewportDimensions = _dimensions;
}

const glm::ivec2& Viewport::ViewportDimensions() const {
	return m_viewportDimensions;
}



void Viewport::ViewportPosition(const glm::ivec2& _position) {
	m_viewportPosition = _position;
}

const glm::ivec2& Viewport::ViewportPosition(const glm::ivec2& _position) const {
	return m_viewportPosition;
}



const int& Viewport::ViewportRenderOrder() const {
	return m_renderOrder;
}

void Viewport::ViewportRenderOrder(const int& _order) {
	m_renderOrder = _order;
}



// - navigation -------------------------------------------------------

bool Viewport::IsViewportMovable() const {
	return m_vpIsMovable;
}

void Viewport::SetViewportMovable(bool _setting) {
	m_vpIsMovable = _setting;
}

void Viewport::OnScroll() {
	if (!m_vpIsMovable) return;

}

void Viewport::OnMouseClickDrag() {
	if (!m_vpIsMovable) return;

}

void Viewport::OnInput() {
	
	if (!m_vpIsMovable) return;
	InputSystem& is = Core::GetInstance().GetInputSystem();
	
	// aliases here.
	InputSystem::INPUT_KEY fwd		{ InputSystem::KEYBOARD_W };
	InputSystem::INPUT_KEY back		{ InputSystem::KEYBOARD_S };
	InputSystem::INPUT_KEY left		{ InputSystem::KEYBOARD_A };
	InputSystem::INPUT_KEY right	{ InputSystem::KEYBOARD_D };
	float moveSpeed					{ 3.0f };
	double dt						{ Core::DeltaTime() };


	// check for inputs
	glm::vec2 inputVector{};
	inputVector.y = static_cast<int>(is.IsKeyHeld(fwd)) - static_cast<int>(is.IsKeyHeld(back));
	inputVector.x = static_cast<int>(is.IsKeyHeld(right)) - static_cast<int>(is.IsKeyHeld(left));
	inputVector = glm::normalize(inputVector);


	m_position.x += inputVector.x * moveSpeed;
	m_position.y += inputVector.y * moveSpeed;
	m_transformDirty = true;
}



// - camera transform -------------------------------------------------

void Viewport::Position(const glm::vec3& _position) {
	if (_position != m_position) {
		m_transformDirty = true;
		m_position = _position;
	}
}

const glm::vec3& Viewport::Position() const {
	return m_position;
}



void Viewport::Rotation(const glm::quat& _rotation) {
	if (_rotation != m_rotation) {
		m_transformDirty = true;
		m_rotation = _rotation;
	}
}

const glm::quat& Viewport::Rotation() const {
	return m_rotation;
}

void Viewport::RotationEuler(const glm::vec3& _rotationEulerDeg) {
	Rotation(glm::quat(_rotationEulerDeg));
}

glm::vec3 Viewport::RotationEuler() const {
	return glm::degrees(glm::eulerAngles(Rotation()));
}



// - camera projection ------------------------------------------------

void Viewport::FieldOfView(float _verticalFovDeg) {
	if (m_verticalFOV != _verticalFovDeg) {
		m_projectionDirty = true;
		m_verticalFOV = _verticalFovDeg;
	}
}

const float& Viewport::FieldOfView() const{
	return m_verticalFOV;
}



void Viewport::NearClip(float _nearClip) {
	if (_nearClip != m_nearClip) {
		m_projectionDirty = true;
		m_nearClip = _nearClip;
	}
}

const float& Viewport::NearClip() const {
	return m_nearClip;
}



void Viewport::FarClip(float _farClip) {
	if (_farClip != m_farClip) {
		m_projectionDirty = true;
		m_farClip = _farClip;
	}
}

const float& Viewport::FarClip() const {
	return m_farClip;
}



void Viewport::AspectRatio(float _aspectRatio) {
	if (m_aspectRatio != _aspectRatio) {
		m_projectionDirty = true;
		m_aspectRatio = _aspectRatio;
	}
}

const float& Viewport::AspectRatio() const {
	return m_aspectRatio;
}



// - dynamic hidden functions ------------------------------------------

void Viewport::Resize() {
}

void Viewport::Move() {

}
