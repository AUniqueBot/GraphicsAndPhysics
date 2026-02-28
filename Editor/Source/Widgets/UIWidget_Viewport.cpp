#include <Widgets/UIWidget_Viewport.h>
#include <arch/systems/sys_render_modules/sys_render_viewport.h>
#include <arch/components/comp_transform.h>
#include <arch/components/comp_light.h>
#include <glm/gtx/matrix_decompose.hpp>


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
	if (!sizeIm.x || !sizeIm.y || !rt.VerifyFBOCompleteness()) return;

	if (rt.GetColorAttachmentCount() == 0) {
		LOG_WARN("Render Target has no color attachments");
		return;
	}
	const unsigned col = rt.GetColorAttachmentTextureID(0);
	assert(glIsTexture(col));
	ImGui::Image(
		(void*)(intptr_t)col, 
		sizeIm, 
		// image is flipped
		ImVec2(0, 1), // top-left in texture
		ImVec2(1, 0)  // bottom-right in texture
	);

	m_imgMin = ImGui::GetItemRectMin();
	m_imgMax = ImGui::GetItemRectMax();

	RenderGizmo();
	RenderLightHelpers();
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

void UIWidget_Viewport::RenderGizmo() const {
	ImGuizmo::SetGizmoSizeClipSpace(0.15f);
	ImGuizmo::BeginFrame();  // Call this before using ImGuizmo

	ImGuizmo::SetOrthographic(false);  // Set to true if using orthographic projection
	ImGuizmo::SetDrawlist();
	// Get the size of the child window
	ImVec2 viewportMin{ ImGui::GetItemRectMin() };
	ImVec2 viewportMax{ ImGui::GetItemRectMax() };


	ImVec2 panelSize{
		(viewportMax.x - viewportMin.x),
		(viewportMax.y - viewportMin.y)
	};

	if (!panelSize.x || !panelSize.y) return;


	ImGuizmo::SetRect(
		viewportMin.x,
		viewportMin.y,
		panelSize.x,
		panelSize.y
	);

}

void UIWidget_Viewport::RenderLightHelpers()  {
	Core& c				{ *ApplicationCore() };
	EntityRegistry& er	{ c.GetRegistry() };
	const auto& compPool = er.GetComponentPool<Light>();
	if (!compPool)return;

	glm::vec2 vp { m_viewportPointer->ViewportDimensions() };
	glm::mat4 clipMtx {
		m_viewportPointer->ProjectionMatrix() *
		glm::inverse(m_viewportPointer->CameraMatrix())
	};


	const UI_Core& uic { *UICore() };
	
	// - icons here -----------------------------------
	GLuint directionalLight		{ uic.GetIcon("directional light") };
	GLuint areaLight			{ uic.GetIcon("area light") };
	GLuint mainLight			{ uic.GetIcon("main light") };
	GLuint pointLight			{ uic.GetIcon("point light") };




	for (const auto& light: compPool->Data()) {
		EntityID enttId = light.GetEntityID();
		if (!enttId.IsValid()) continue;
		const auto& entity	{ *er.GetEntity(enttId) };
		const auto& trs = *entity.GetComponent<Transform>();
		std::string name{entity.Name()};
		// get transform position.

		// light.Type();
		// switch case based on this.

		// for now all lights use the same icon. (light)

		// convert position to position in screen.
		glm::vec4 clip = clipMtx * glm::vec4(trs.Position(), 1.0f);

		if (clip.w <= 0.0f)
			continue; // behind camera
	
		// NDC 
		glm::vec3 ndc = glm::vec3(clip.x, clip.y, clip.z) / clip.w;

		// skip out of clip range
		if (ndc.x < -1 || ndc.x > 1 ||
			ndc.y < -1 || ndc.y > 1 ||
			ndc.z < -1 || ndc.z > 1)
			continue;

		ImVec2 screen {
			(ndc.x * 0.5f + 0.5f) * vp.x,
			(1.0f - (ndc.y * 0.5f + 0.5f))* vp.y
		};
		name += "##";
		name += static_cast<unsigned long>(enttId);



		GLuint currentLightIcon{};

		switch (light.Type()) {
		
		case LightType::POINT:
			currentLightIcon = pointLight;
			break;		

/*		case LightType::AREA : 
			currentLightIcon = areaLight;
			break;	*/	
		case LightType::SPOT: 
			break;		
		case LightType::DIRECTIONAL : 
			currentLightIcon = directionalLight;
			break;		
		case LightType::AMBIENT:
			currentLightIcon = mainLight;
			break;		
		//case LightType::SUN : 
		//	break;
		}


		

		if (currentLightIcon != 0) {
			ImVec2 buttonSize = {32, 32};
			screen.x -= buttonSize.x * 0.5f;
			screen.y += buttonSize.y * 0.5f;
			ImGui::SetCursorPos(screen);

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));        // background
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0)); // hovered
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));  // active
			if (ImGui::ImageButton(name.c_str(), 
					(void*)(intptr_t)currentLightIcon, 
				ImVec2((float)32, (float)32),
				ImVec2(0, 0), ImVec2(1, 1)
			)) {
				er.SelectEntity(enttId);
			}
			ImGui::PopStyleColor(3);
		}
		else {
			ImVec2 buttonSize = ImGui::CalcTextSize(name.c_str());
			buttonSize.x += ImGui::GetStyle().FramePadding.x * 2.0f;
			buttonSize.y += ImGui::GetStyle().FramePadding.y * 2.0f;
			screen.x -= buttonSize.x * 0.5f;
			screen.y -= buttonSize.y * 0.5f;
			ImGui::SetCursorPos(screen);

			if (ImGui::Button(name.c_str())) {
				er.SelectEntity(enttId);
			}
		}
	}
}

void UIWidget_Viewport::Update() {
	if (!ApplicationCore()) return;
	Core& core{ *ApplicationCore() };
	UpdateGizmo();
	
	if (WidgetIsHoveredOver()) {
		if (ImGui::IsKeyDown(ImGuiKey_LeftAlt)) {
			core.GetInputRouter().ReleaseOwnership("UI");
		}
		else {
			PickObjectFromScreen();
		}
	}	

}


void UIWidget_Viewport::PickObjectFromScreen() {
	// call the viewport to pick from screen.
			// get the new window resolution.
		
	// on click pick.
	// this function does not care about
	if (!m_viewportPointer || !m_viewportPointer->GetRenderTarget()) return;
	// disable when gizmo is being hovered to disable selection when attempting to 
	// click over a gizmo at a spot where there is no object.
	if (m_isInteractingGizmo) return; 
	Viewport& vp{ *m_viewportPointer };
		
	// mouse position to image position

	ImVec2 mouse = ImGui::GetMousePos();
	ImVec2 viewportMin = ImGui::GetItemRectMin();
	ImVec2 viewportMax = ImGui::GetItemRectMax();
	glm::vec2 local{};
	local.x = mouse.x - m_imgMin.x;
	local.y = mouse.y - m_imgMin.y;

	if (local.x < 0 || local.y < 0 ||
		local.x > (m_imgMax.x - m_imgMin.x) ||
		local.y > (m_imgMax.y - m_imgMin.y))
	{
		return;
	}
	local.y = (m_imgMax.y - m_imgMin.y) - local.y;
	glm::vec2 framebufferSize = vp.GetRenderTarget()->Resolution();
	glm::vec2 viewportSize = {
		m_imgMax.x - m_imgMin.x,
		m_imgMax.y - m_imgMin.y
	};
	glm::vec2 pixel{};
	pixel.x = local.x * (framebufferSize.x / viewportSize.x);
	pixel.y = local.y * (framebufferSize.y / viewportSize.y);


	
	if (!ApplicationCore()) {
		return;
	}




	if (ImGui::IsMouseClicked(0)) {

		unsigned picked = vp.GetRenderTarget()->PickPixel(pixel, C_RENDER_OBJECTID);
		LOG_INFO("Picked ID: "<< picked );
		EntityRegistry& registry = ApplicationCore()->GetRegistry();
		EntityView entity = registry.GetEntity(picked);
		
		ApplicationCore()->GetRegistry().SelectEntity(
			entity ? entity->GetID() : 
			EntityID::ENTITYID_INVALID
		);
	}

}

void UIWidget_Viewport::ManageInput() const {
	// if the mouse is hovering over the gizmo or is interacting with the gizmo, all input is routed through 

}

void UIWidget_Viewport::UpdateGizmo() {

	if (!m_viewportPointer) return;
	Core* c								{ ApplicationCore() };
	if (!c) return;
	EntityRegistry registry = c->GetRegistry();

	Viewport& vp						{ *m_viewportPointer };
	EntityID selectedEntityID			{ registry.SelectedEntity() };
	bool objectIsSelected				{ selectedEntityID != EntityID::ENTITYID_INVALID };
	if (!objectIsSelected) return;


	

	ImGuizmo::Enable(objectIsSelected);
	m_currentGizmoOperation = 
		ImGui::IsKeyPressed(ImGuiKey_W, false) ? ImGuizmo::TRANSLATE : 
		ImGui::IsKeyPressed(ImGuiKey_E, false) ? ImGuizmo::ROTATE : 
		ImGui::IsKeyPressed(ImGuiKey_R, false) ? ImGuizmo::SCALE :
		m_currentGizmoOperation;


	EntityView e				{ registry.GetEntity(selectedEntityID) };
	Transform&	trsComponent	{ *e->GetComponent<Transform>() };
	glm::mat4 trsMtx			{ trsComponent.WorldTransformMtx() };
	


	ImGuizmo::Manipulate(
		glm::value_ptr(glm::inverse(vp.CameraMatrix())),
		glm::value_ptr(vp.ProjectionMatrix()),
		m_currentGizmoOperation, 
		ImGuizmo::WORLD, 
		glm::value_ptr(trsMtx)
	);
	m_isInteractingGizmo = ImGuizmo::IsUsing() || ImGuizmo::IsOver();
	if (ImGuizmo::IsUsing()) {

		glm::vec3 pos {};
		glm::quat rot {};
		glm::vec3 scl {};

		glm::vec3 skew{};
		glm::vec4 perspective{};


		// ImGuizmo version is broken.
		glm::decompose(
			trsMtx, 
			scl, rot, pos, 
			skew, perspective
		);

		trsComponent.Position(pos);
		trsComponent.Rotation(rot);
		trsComponent.Scale(scl);

	}

}

