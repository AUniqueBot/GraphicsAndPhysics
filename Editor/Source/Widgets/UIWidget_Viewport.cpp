#include <Widgets/UIWidget_Viewport.h>
#include <arch/systems/sys_render_modules/sys_render_viewport.h>
#include <arch/components/comp_transform.h>


UIWidget_Viewport::UIWidget_Viewport(std::string _widgetName, std::shared_ptr<Viewport> _target)
	: UIWidget(_widgetName), m_viewportPointer{_target} {

}

void UIWidget_Viewport::Init() {

}

void UIWidget_Viewport::Draw() {
	// only contains the render target.
	//ImGuizmo::SetOrthographic(false);
	using namespace ImGui;
	// aliases and early exits.
	if (!m_viewportPointer) return;
	Viewport & vp = *m_viewportPointer.get();
	if (!vp.GetRenderTarget()) return;
	RenderTarget& rt = *vp.GetRenderTarget();

	ResizeCallback();
	if (!rt.VerifyFBOCompleteness()) return;
		

	// unlike the other widgets, clicking here is allowed.
	const glm::ivec2& sizeGL = vp.ViewportDimensions();
	ImVec2 sizeIm = { static_cast<float>(sizeGL.x), static_cast<float>(sizeGL.y) };
	if (!sizeIm.x || !sizeIm.y) return;

			
	const unsigned col = rt.GetColorAttachmentTextureID(0);
	assert(glIsTexture(col));
	ImGui::Image(
		(void*)(intptr_t)col, 
		sizeIm, 
		// image is flipped
		ImVec2(0, 1), // top-left in texture
		ImVec2(1, 0)  // bottom-right in texture
	);

	
	if (BeginDragDropTarget()) {
		EndDragDropTarget();
	}

}

void UIWidget_Viewport::Exit() {
}

void UIWidget_Viewport::ResizeCallback() const {
	// get the new window resolution.
	if (!m_viewportPointer || !m_viewportPointer->GetRenderTarget()) return;
	Viewport& vp	{ *m_viewportPointer };
		
	static ImVec2 lastSize{};
	ImVec2 currentSize = ImGui::GetContentRegionAvail();
	if (!currentSize.x || !currentSize.y) return;// resize to dims 0 willl result in an empty buffer.

	if (currentSize.x != lastSize.x || currentSize.y != lastSize.y) {

		vp.ViewportDimensions({ currentSize.x, currentSize.y });
		vp.AspectRatio(currentSize.x / currentSize.y);
		lastSize = currentSize;
	}

}

void UIWidget_Viewport::Update() {
	if (WidgetIsHoveredOver()) {
		LOG_INFO("Hovering over viewport widget");
		PickObjectFromScreen();
	}

	UpdateGizmo();
}


void UIWidget_Viewport::PickObjectFromScreen() const {
	// call the viewport to pick from screen.
			// get the new window resolution.
		
	// on click pick.
	// this function does not care about
	if (!m_viewportPointer || !m_viewportPointer->GetRenderTarget()) return;
	Viewport& vp{ *m_viewportPointer };
		
	// mouse position to image position

	ImVec2 mouse = ImGui::GetMousePos();
	ImVec2 viewportMin = ImGui::GetItemRectMin();
	ImVec2 viewportMax = ImGui::GetItemRectMax();
	glm::vec2 local{};
	local.x = mouse.x - viewportMin.x;
	local.y = mouse.y - viewportMin.y;
	if (local.x < 0 || local.y < 0 ||
		local.x > (viewportMax.x - viewportMin.x) ||
		local.y > (viewportMax.y - viewportMin.y))
	{
		return;
	}
	local.y = (viewportMax.y - viewportMin.y) - local.y;
	glm::vec2 framebufferSize = vp.GetRenderTarget()->Resolution();
	glm::vec2 viewportSize = {
		viewportMax.x - viewportMin.x,
		viewportMax.y - viewportMin.y
	};
	glm::vec2 pixel{};
	pixel.x = local.x * (framebufferSize.x / viewportSize.x);
	pixel.y = local.y * (framebufferSize.y / viewportSize.y);


	unsigned picked = vp.GetRenderTarget()->PickPixel(pixel, C_RENDER_OBJECTID);

	EntityViewConst entity = ApplicationCore()->Registry().Get(picked);
	LOG_INFO("Value received from read: " << picked);
	if (entity) {
		LOG_INFO("Object Picked: " << entity->Name());

	}
	if (ImGui::IsMouseClicked(0)) {
		UICore()->SelectedEntity(picked);
	}

}

void UIWidget_Viewport::UpdateGizmo() {

	if (!m_viewportPointer) return;
	auto& uic							{ *UICore() };
	auto& c								{ *ApplicationCore() };

	Viewport& vp						{ *m_viewportPointer };
	EntityID selectedEntityID			{ uic.SelectedEntity() };
	bool objectIsSelected				{ uic.SelectedEntity() != EntityID::ENTITYID_INVALID };
	if (!objectIsSelected) return;

	ImGuizmo::SetGizmoSizeClipSpace(0.15f);
	ImGuizmo::BeginFrame();  // Call this before using ImGuizmo

	ImGuizmo::SetOrthographic(false);  // Set to true if using orthographic projection
	ImGuizmo::SetDrawlist();
	// Get the size of the child window
	ImVec2 viewportMin	{ ImGui::GetItemRectMin() };
	ImVec2 viewportMax	{ ImGui::GetItemRectMax() };


	ImVec2 panelSize{
		(viewportMax.x - viewportMin.x),
		(viewportMax.y - viewportMin.y)
	};
	ImGuizmo::SetRect(
		viewportMin.x,
		viewportMin.y,
		panelSize.x,
		panelSize.y
	);




	LOG_INFO(
		"viewport pos: ["<< viewportMin.x <<", " << viewportMin.y <<"] " << 
		"viewport size: [" << panelSize.x << ", " << panelSize.y << "] ");


	ImGuizmo::Enable(objectIsSelected);
	m_currentGizmoOperation = 
		ImGui::IsKeyPressed(ImGuiKey_W, false) ? ImGuizmo::TRANSLATE : 
		ImGui::IsKeyPressed(ImGuiKey_E, false) ? ImGuizmo::ROTATE : 
		ImGui::IsKeyPressed(ImGuiKey_R, false) ? ImGuizmo::SCALE :
		m_currentGizmoOperation;


	EntityView e				{ c.Registry().Get(selectedEntityID) };
	Transform&	trsComponent	{ *e->GetComponent<Transform>() };
	glm::mat4 trsMtx			{ trsComponent.WorldTransformMtx() };
	
	
	
	ImGuizmo::Manipulate(
		glm::value_ptr(vp.CameraMatrix()),
		glm::value_ptr(vp.ProjectionMatrix()),
		m_currentGizmoOperation, ImGuizmo
		::WORLD, 
		glm::value_ptr(trsMtx)
	);


	if (ImGuizmo::IsUsing()) {
		glm::vec3 pos {};
		glm::quat rot {};
		glm::vec3 scl {};

		glm::vec3 skew {};
		glm::vec4 perspective{};
	

		ImGuizmo::DecomposeMatrixToComponents(
			glm::value_ptr(trsMtx), 
			glm::value_ptr(pos), 
			glm::value_ptr(rot), 
			glm::value_ptr(scl)
		);

		trsComponent.Position(pos);
		trsComponent.Rotation(rot);
		trsComponent.Scale(scl);

	}

}

