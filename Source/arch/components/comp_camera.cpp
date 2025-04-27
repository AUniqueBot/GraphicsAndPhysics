#include <pch.h>
#include <arch/components/comp_camera.h>

int Camera::m_activeCameraId    {};
int Camera::m_cameraCount       {};

int Camera::ActiveCameraID() {
    return m_activeCameraId;
}

