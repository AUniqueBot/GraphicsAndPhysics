#include <pch.h>
#include <arch/systems/sys_render_modules/sys_render_viewport.h>
#include <arch/core.h>


// - general ----------------------------------------------------------

Viewport::Viewport(ViewportID _generatedId) : m_vpId{ _generatedId }, m_projectionDirty{ true }, m_transformDirty{ true }  {
}

void Viewport::Init() {
	UpdateAttributes();
}

void Viewport::Start() {

}

void Viewport::Render() {
	
}




void Viewport::Update() {
	OnInput();
	OnMouseMove();

	UpdateAttributes();
}



// - matrix -----------------------------------------------------------

void Viewport::UpdateAttributes() {
	if (m_transformDirty) {
		glm::mat4 pos = glm::translate(
			glm::mat4	{ 1.f },
			m_position
		);

		glm::mat4 rot	{ m_rotation };
		glm::mat4 flipZ = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, -1));

		m_viewportMatrix = pos * rot * flipZ;
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
	if (m_viewportDimensions == _dimensions) return;
	m_viewportDimensions = _dimensions;
	Resize();
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

void Viewport::OnMouseMove() {
	if (!m_vpIsMovable) return;


	InputSystem& is = Core::GetInstance().GetInputSystem();


	InputSystem::INPUT_MOUSE_BUTTON actionButton = InputSystem::MOUSE_LEFT;
	if (is.IsMouseButtonClicked(actionButton)) {
		//LOG_INFO("LMB Clicked");
	}
	if (is.IsMouseButtonReleased(actionButton)) {
		//LOG_INFO("LMB Released");
	}
	if (!is.IsMouseButtonHeld(InputSystem::MOUSE_LEFT)) {
		return;
	}



	glm::vec2 delta = is.GetMouseDelta();

	if (!delta.x && !delta.y) return;

	const glm::vec3 forwardVec	{ 0, 0, 1 };
	const glm::vec3 upVec		{ 0, 1, 0 };

	// sensitivity tuning (adjust as needed)
	float sensitivity = 0.002f;
	float yaw			= -delta.x * sensitivity;
	float pitch			= -delta.y * sensitivity;

	glm::vec3 forward	= m_rotation * forwardVec; 
	glm::vec3 right		= glm::normalize(glm::cross(forward, upVec));

	glm::quat yawRot	= glm::angleAxis(yaw, upVec);
	glm::quat pitchRot	= glm::angleAxis(pitch, right);

	m_rotation			= glm::normalize(yawRot * pitchRot * m_rotation);

	glm::vec3 newForward = m_rotation * glm::vec3(0, 0, 1);

	m_transformDirty = true;

	
}

void Viewport::OnInput() {
	
	if (!m_vpIsMovable) return;


	InputSystem& is = Core::GetInstance().GetInputSystem();
	
	// aliases here.
	const InputSystem::INPUT_KEY fwd		{ InputSystem::KEYBOARD_W };
	const InputSystem::INPUT_KEY back		{ InputSystem::KEYBOARD_S };
	const InputSystem::INPUT_KEY left		{ InputSystem::KEYBOARD_A };
	const InputSystem::INPUT_KEY right		{ InputSystem::KEYBOARD_D };
	const InputSystem::INPUT_KEY up			{ InputSystem::KEYBOARD_SPACE };
	const InputSystem::INPUT_KEY down		{ InputSystem::KEYBOARD_LCONTROL };

	float moveSpeed					{ 10.0f };
	float dt						{ static_cast<float>(Core::DeltaTime()) };


	// check for inputs
	glm::vec3 inputVector			{ 0.0f, 0.0f, 0.0f };


	if (is.IsKeyHeld(right) || is.IsKeyHeld(left)) {
		inputVector.x  = static_cast<float>(static_cast<int>(is.IsKeyHeld(right)) - static_cast<int>(is.IsKeyHeld(left)));
		LOG_INFO("keyboard input detected! lateral");
	}

	if (is.IsKeyHeld(up) || is.IsKeyHeld(down)) {
		inputVector.y  = static_cast<float>(static_cast<int>(is.IsKeyHeld(up)) - static_cast<int>(is.IsKeyHeld(down)));

		LOG_INFO("keyboard input detected! vertical");
	}
	if (is.IsKeyHeld(fwd) || is.IsKeyHeld(back)) {
		inputVector.z  = static_cast<float>(static_cast<int>(is.IsKeyHeld(fwd)) - static_cast<int>(is.IsKeyHeld(back)));

		LOG_INFO("keyboard input detected! directional");
	}

	if (inputVector.x || inputVector.y || inputVector.z) {
		inputVector = glm::normalize(inputVector);
		inputVector = m_rotation * inputVector;

		m_position.x += inputVector.x * moveSpeed * dt;
		m_position.y += inputVector.y * moveSpeed * dt;
		m_position.z += inputVector.z * moveSpeed * dt;

		m_transformDirty = true;
	}

}

Viewport::ViewportID Viewport::ID() const {
	return m_vpId;
}

std::shared_ptr<RenderTarget> Viewport::GetRenderTarget() {
	return m_renderTarget;
}

std::shared_ptr<const RenderTarget> Viewport::GetRenderTarget() const {
	return m_renderTarget;
}

void Viewport::SetRenderTarget(std::shared_ptr<RenderTarget> _renderTarget) {
	m_renderTarget = _renderTarget;
}

void Viewport::PickFromViewport(glm::vec2 _position) const {
	
	
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
	if (!m_renderTarget) return;
	RenderTarget& rt = *m_renderTarget;
	rt.Resize(m_viewportDimensions);
}

void Viewport::Move() {

}
