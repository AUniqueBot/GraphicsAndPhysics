#pragma once
#include <pch.h>
#include <arch/systems/sys_render_modules/sys_render_renderTarget.h>
class Viewport {
public:
	enum FACETORENDER {
		FRONT = GL_FRONT,
		BACK = GL_BACK,
		FRONTANDBACK = GL_FRONT_AND_BACK
	};

	enum RENDERMODE {
		POINT = GL_POINT,
		LINE = GL_LINE,
		FILL = GL_FILL
	};

public:
	using ViewportID = uint32_t;
public:
	Viewport(ViewportID _generatedId);


	void Init();
	void Start();

	void Render();

	RENDERMODE GetRenderMode() const;
	void SetRenderMode(RENDERMODE _mode) ;
	FACETORENDER GetFaceToRender() const;
	void SetFaceToRender(FACETORENDER _faces);

	void Update();




	// - camera transform -----------------------------
	void Position(const glm::vec3& _position);
	const glm::vec3& Position() const;

	void Rotation(const glm::quat& _rotation);
	const glm::quat& Rotation() const;

	void RotationEuler(const glm::vec3& _rotationEulerDeg);
	glm::vec3 RotationEuler() const;

	const glm::vec3& Forward() const;


	// - camera projection ----------------------------
	void FieldOfView(float _verticalFovDeg);
	const float& FieldOfView() const;

	void NearClip(float _nearClip);
	const float& NearClip() const;

	void FarClip(float _farClip);
	const float& FarClip() const;

	void AspectRatio(float _aspectRatio);
	const float& AspectRatio() const;

	// - matrix ---------------------------------------
	const glm::mat4& CameraMatrix() const;
	const glm::mat4& ProjectionMatrix() const;
	
	// - viewport management --------------------------
	void ViewportDimensions(const glm::ivec2& _dimensions);
	const glm::ivec2& ViewportDimensions() const;

	void ViewportPosition(const glm::ivec2& _position);
	const glm::ivec2& ViewportPosition(const glm::ivec2& _position) const;

	const int& ViewportRenderOrder() const;
	void ViewportRenderOrder(const int& _order);

	// - navigation -----------------------------------
	
	bool IsViewportMovable() const;
	void SetViewportMovable(bool _setting);
	void OnMouseMove();
	void OnInput();


	ViewportID ID() const;


	// - render target handling -----------------------
	std::shared_ptr<RenderTarget> GetRenderTarget();
	std::shared_ptr<const RenderTarget> GetRenderTarget() const;
	void SetRenderTarget(std::shared_ptr<RenderTarget> _renderTarget);

private:
	void UpdateAttributes();

	// - dynamic hidden functions ---------------------
	void Resize(); // dynamic resize function 
	void Move();  // dynamic move function.

private:
	// - viewport window settings ---------------------
	bool m_isActive					{ true };
	int m_renderOrder				{}; // affects render order. higher means render later.
	glm::ivec2 m_viewportPosition	{};
	glm::ivec2 m_viewportDimensions	{ 1280, 720 };

	ViewportID m_vpId;
	// - render target ----------------------------------
	std::shared_ptr<RenderTarget> m_renderTarget;

private:

	bool m_vpIsMovable				{ true };

	// - camera transform settings ---------------------
	glm::vec3 m_position			{};
	glm::quat m_rotation			{ 1, 0, 0, 0 }; // better to not declare this.
	glm::mat4 m_viewportMatrix		{ 1.f };
	
	bool m_transformDirty			{ true };
	
	// assuming forward is +y

	// - camera projection settings --------------------
	float m_nearClip				{ 0.001f };
	float m_farClip					{ 1000.0f };

	float m_aspectRatio				{ 16.f/9.f };
	
	bool m_isOrthographic			{ false };
	float m_verticalFOV				{ 60.0f };
	float m_orthoScale				{ 1.0f };


	bool m_projectionDirty			{ true };
	glm::mat4 m_projectionMatrix	{ 1.f };

	FACETORENDER m_faceToRender		{ FRONTANDBACK };
	RENDERMODE	m_renderMode		{ FILL };
};