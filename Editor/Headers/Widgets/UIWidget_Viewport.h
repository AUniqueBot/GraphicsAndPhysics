#pragma once
#include <UI_Widget.h>
#include <ImGuizmo.h>


// - declaration only ------------------
class Viewport;
class EntityID;


class UIWidget_Viewport : public UIWidget {
public:
	enum class GizmoState {
		IsHovering,
		IsHandling,
		NONE
	};


public:
	UIWidget_Viewport(std::string _widgetName, std::shared_ptr<Viewport> _target);
	~UIWidget_Viewport() = default;


	void Init() override;
	void Draw() override;
	void Exit() override;


	void Update() override;

	void ResizeCallback() const;
	void RenderGizmo() const;



	void UpdateGizmo();
	void PickObjectFromScreen();
	void ManageInput() const;

private:
	std::shared_ptr<Viewport> m_viewportPointer;
	bool m_isInteractingGizmo						{ false };
	double m_clickDeltaTime							{};
	double m_clickTime								{ 0.01 };
	
	ImGuizmo::OPERATION m_currentGizmoOperation		{ ImGuizmo::TRANSLATE };

};