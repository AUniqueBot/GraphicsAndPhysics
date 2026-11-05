#pragma once
#include <pch.h>
#include <arch/common/component.h>


namespace CameraConstants {
	enum class TonemappingModes : int {
		LINEAR = 0,
		REINHARD = 1,
		ACES_FILMIC = 2
	};


	enum class RenderTargetLocations : int {
		OBJECT_ID = 0,
		COLOR = 1
	};

};


struct CameraData {

	// - generic metadata ----------------------------
	glm::vec3 m_position;
	glm::quat m_rotation;
	glm::vec2 m_clipPlanes;

	// - rendering metadata --------------------------
	float m_exposure;
	float m_aspectRatio;
	
	bool m_isOrthographic;
	float m_orthographicSize;
	float m_fov;
	CameraConstants::TonemappingModes m_tonemapMode;


	glm::mat4 ViewMatrix();
	glm::mat4 InverseViewMatrix();


};




class Camera : public Component {
private:

public:

	

public:
	
	// - generic -------------------------------	
	void Init() override;
	void Update();
	void End() override;

	static void Register() { LOG_INFO("Camera Register Override"); }


	// - camera handling ------------------------
	static int ActiveCameraID();

	// - clip plane distance --------------------
	void ClipPlanes(glm::vec2);
	void ClipPlanes(float, float);
	void NearClipPlane(float _dist);
	void FarClipPlane(float _dist);

	glm::vec2 ClipPlanes() const;
	float NearClipPlane() const;
	float FarClipPlane() const;

	// - field of view --------------------------
	void FieldOfViewVertical(float _fov);
	void FieldOfViewHorizontal(float _fov);

	float FieldOfViewVertical() const;
	float FieldOfViewHorizontal() const;

	// - aspect ratio ---------------------------
	void AspectRatio(glm::vec2 _ratio);
	void AspectRatio(float, float);
	void AspectRatio(float); // pure ratio variant
	void AspectRatioX(float _val);
	void AspectRatioY(float _val);

	glm::vec2 AspectRatio() const;

	// - projection settings --------------------
	void OrthogonalProjectionEnabled(bool);
	bool OrthogonalProjectionEnabled() const;


	// - projection matrix ----------------------
	glm::mat4 ProjectionMatrix() const { return m_projectionMatrix; }


	// - rendering settings ---------------------
	const float& Exposure() const;
	void Exposure(float _value);

	const CameraConstants::TonemappingModes& TonemapMode() const;
	void TonemapMode(const CameraConstants::TonemappingModes& _mode);



private:
	void UpdateProjectionMatrix();
private:
	static int m_activeCameraId;
	static int m_cameraCount;
	static int m_staticCounter;
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


	float m_exposure					{ 1.0 };
	CameraConstants::TonemappingModes m_tonemapMode	{ CameraConstants::TonemappingModes::LINEAR };
};


