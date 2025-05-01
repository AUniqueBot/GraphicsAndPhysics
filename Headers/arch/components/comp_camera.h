#pragma once
#include <pch.h>
#include <arch/common/component.h>


class Camera : public Component {
private:

public:
	
	// - generic -------------------------------	
	void Init();
	void Update();
	void Terminate();

	// - camera handling ------------------------
	static int ActiveCameraID();

	// - clip plane distance --------------------
	void SetClipPlaneDistance(glm::vec2);
	void SetClipPlaneDistance(float, float);
	void SetNearClipPlaneDistance(float _dist);
	void SetFarClipPlaneDistance(float _dist);

	glm::vec2 GetClipPlaneDistance() const;
	float GetNearClipPlaneDistance() const;
	float GetFarClipPlaneDistance() const;

	// - field of view --------------------------
	void SetFieldOfViewVertical(float _fov);
	void SetFieldOfViewHorizontal(float _fov);

	float GetFieldOfViewVertical() const;
	float GetFieldOfViewHorizontal() const;

	// - aspect ratio ---------------------------
	void SetAspectRatio(glm::vec2 _ratio);
	void SetAspectRatio(float, float);
	void SetAspectRatio(float); // pure ratio variant
	void SetAspectRatioX(float _val);
	void SetAspectRatioY(float _val);

	glm::vec2 GetAspectRatio() const;

	// - projection settings --------------------
	void SetOrthogonalProjection(bool);
	bool GetOrthogonalProjection() const;


	// - projection matrix ----------------------
	glm::mat4 GetProjectionMatrix() const { return m_projectionMatrix; }
private:
	void UpdateProjectionMatrix();
private:
	static int m_activeCameraId;
	static int m_cameraCount;

	int m_cameraId						{};


	// clip plane
	float m_nearClipPlaneDistance		{ 0.00001f };
	float m_farClipPlaneDistance		{ 10000.f };
	// perspective
	float m_verticalFov					{ 55.0f };
	glm::vec2 m_aspect					{ 16.f, 9.f };
	bool m_matchScreenAspectRatio		{ false };

	// get window size lol
	bool m_orthogonalProjection			{ false };

	bool m_componentDataDirty			{ true };

	glm::mat4 m_projectionMatrix		{};
};