#include <pch.h>
#include <graphics/gfx_glfwCustomCallbackFunctions.h>
#include <arch/core.h>
#include <UI_Core.h>
#include <arch/core/core_inputRouter.h>
#include <applicationcontext.h>





inline constexpr const unsigned C_VERSION_MAJOR = 4;
inline constexpr const unsigned C_VERSION_MINOR = 6;




static void InitInputs(InputRouter& _router, Core& _core, UI_Core& _uiCore) {

	InputSystem& is{ _core.GetInputSystem() };

	_router.RegisterSystem(is, "Core");
	_router.RegisterSystem(_uiCore, "UI");
	std::vector<std::string> clientNames{ _router.GetAllClients() };

	LOG_INFO("router registered input systems");
	for (const std::string& clientName : clientNames) {
		LOG_INFO(clientName);
	}

}

// - main -----------------------------------------------------------------------------------------------------
int main() {
	ApplicationContextProps props = ApplicationContextProps::GenerateWindowConfig("./Config/applicationconfig.cfg");

	ApplicationContext app(props);

	app.Init();
	app.Update();
	app.Cleanup();

}









