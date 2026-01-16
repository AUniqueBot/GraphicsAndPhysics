#include <Widgets/UIWidget_Viewport.h>
#include <arch/systems/sys_render_modules/sys_render_viewport.h>
#include <imgui.h>



	UIWidget_Viewport::UIWidget_Viewport(std::string _widgetName, std::shared_ptr<Viewport> _target)
		: UIWidget(_widgetName), m_viewportPointer{_target} {

	}

	void UIWidget_Viewport::Init() {

	}

	void UIWidget_Viewport::Draw() {
		// only contains the render target.
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

			
		const unsigned col = rt.GetColorAttachment(0);
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

		if (WidgetIsHoveredOver()) {
			LOG_INFO("Hovering over viewport widget");
			PickObjectFromScreen();
		}


		// onresize, call the viewport to change along with it's rendertarget.
		
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

	void UIWidget_Viewport::PickObjectFromScreen() const {
		// call the viewport to pick from screen.
				// get the new window resolution.
		
		// on click pick.
		// this function does not care about
		if (!m_viewportPointer || !m_viewportPointer->GetRenderTarget()) return;
		Viewport& vp{ *m_viewportPointer };
		
		unsigned picked = vp.GetRenderTarget()->PickPixel(glm::vec2());

		LOG_INFO("Value received from read: " << picked);


	}

