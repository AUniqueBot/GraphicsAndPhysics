#include <pch.h>
#include <arch/components/comp_camera.h>

int Camera::m_activeCameraId    {};
int Camera::m_cameraCount       {};




void Camera::Update() {
    
    if (m_componentDataDirty) {
        UpdateProjectionMatrix();
        m_componentDataDirty = false;
    }
    
}

int Camera::ActiveCameraID() {
    return m_activeCameraId;
}






// - clip distance -----------------------------------------------------
void Camera::SetClipPlaneDistance(glm::vec2 _dist) {
    SetClipPlaneDistance(_dist);
}
void Camera::SetClipPlaneDistance(float _a, float _b) {
    if (_a < _b) {
        SetNearClipPlaneDistance(_a);
        SetFarClipPlaneDistance(_b);
    }
    else {
        SetNearClipPlaneDistance(_b);
        SetFarClipPlaneDistance(_a);
    }
}

void Camera::SetNearClipPlaneDistance(float _dist) {
    if (m_nearClipPlaneDistance == _dist) return;
    m_nearClipPlaneDistance = _dist;
    m_componentDataDirty = true;
}

void Camera::SetFarClipPlaneDistance(float _dist) {
    if (m_farClipPlaneDistance == _dist) return;
    m_farClipPlaneDistance = _dist;
    m_componentDataDirty = true;
}

glm::vec2 Camera::GetClipPlaneDistance() const {
    return glm::vec2(m_nearClipPlaneDistance, m_farClipPlaneDistance);
}

float Camera::GetNearClipPlaneDistance() const {
    return m_nearClipPlaneDistance;
}

float Camera::GetFarClipPlaneDistance() const {
    return m_farClipPlaneDistance;
}




// - field of view -----------------------------------------------------
void Camera::SetFieldOfViewVertical(float _fov) {
    if (_fov == m_verticalFov) return;
    m_verticalFov = _fov;
    m_componentDataDirty = true;
}

void Camera::SetFieldOfViewHorizontal(float _fov) {
    SetFieldOfViewVertical(_fov / m_aspect.x * m_aspect.y);
}

float Camera::GetFieldOfViewVertical() const {
    return m_verticalFov;
}

float Camera::GetFieldOfViewHorizontal() const {
    return m_verticalFov * m_aspect[0] / m_aspect[1];
}


// - aspect ratio ------------------------------------------------------
void Camera::SetAspectRatio(glm::vec2 _ratio) {
    if (m_aspect == _ratio) return;
    m_aspect = _ratio;
    m_componentDataDirty = true;
}

void Camera::SetAspectRatio(float _x, float _y) {
    SetAspectRatio(glm::vec2(_x, _y));
}

void Camera::SetAspectRatio(float _ratio) {
    if (m_aspect[0] / m_aspect[1] == _ratio) return;
    m_aspect[0] = _ratio;
    m_aspect[1] = 1;
    m_componentDataDirty = true;
}

void Camera::SetAspectRatioX(float _val) {
    if (m_aspect[0] == _val) return;
    m_aspect[1] = _val;
    m_componentDataDirty = true;
}

void Camera::SetAspectRatioY(float _val) {
    if (m_aspect[1] == _val) return;
    m_aspect[1] = _val;
    m_componentDataDirty = true;
}

glm::vec2 Camera::GetAspectRatio() const {
    return m_aspect;
}


// - projection settings -----------------------------------------------
void Camera::SetOrthogonalProjection(bool _s) {
    if (m_orthogonalProjection == _s) return;
    m_orthogonalProjection = _s;
    m_componentDataDirty = true;
}

bool Camera::GetOrthogonalProjection() const {
    return m_orthogonalProjection;
}


// - projection matrix -------------------------------------------------
void Camera::UpdateProjectionMatrix() {
    if (m_orthogonalProjection) {
        // get viewport 
        GLint viewport[4]{};
        glGetIntegerv(
            GL_VIEWPORT,
            viewport
        );

        m_projectionMatrix = glm::ortho(
            0.f, (float)viewport[2],
            0.f, (float)viewport[3],
            m_nearClipPlaneDistance,
            m_farClipPlaneDistance
        );
    }
    else {
        m_projectionMatrix = glm::perspective(
            m_verticalFov,
            m_aspect.x/m_aspect.y,
            m_nearClipPlaneDistance,
            m_farClipPlaneDistance
        );
    }
}

