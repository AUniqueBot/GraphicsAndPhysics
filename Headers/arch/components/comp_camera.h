#pragma once
#include <pch.h>
#include <arch/common/component.h>


class Camera : public Component {
private:
public:

	static int ActiveCameraID();


private:
	static int m_activeCameraId;
	static int m_cameraCount;

	
	int m_cameraId					{};
};