#include <pch.h>
#include <graphics/gfx_glfwCustomCallbackFunctions.h>
#include <arch/core.h>
#include <arch/resources/res_mesh.h>
#include <UI_Core.h>
#include <arch/core/core_inputRouter.h>


const unsigned C_VERSION_MAJOR = 4;
const unsigned C_VERSION_MINOR = 6;


void InitInputs(InputRouter& _router, Core& _core, UI_Core& _uiCore) {

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
	Core& c = Core::GetInstance();
	UI_Core uic{};

	InitInputs(c.GetInputRouter(), c, uic);


	// core to also initialise the timer
	// - GLFW Initialisation ------------------------------------------------------
	// abstract to window.
	glfwInit();
	// setting up opengl version (opengl 4.)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, C_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, C_VERSION_MINOR);

	// tbh mate, idk what this does.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	// create a window
	GLFWwindow* mainWindow = glfwCreateWindow(1920, 1080, "BWOAH", nullptr, nullptr);
	c.SetWindow(mainWindow);

	// failed to create window
	if (!mainWindow) { 
		LOG_ERROR("Bad GLFW Init.");

		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(mainWindow);
	glfwSetInputMode(mainWindow, GLFW_STICKY_KEYS, GLFW_TRUE);



	// - GLEW Initialisation -------------------------------------------------------
	GLenum glewStatus = glewInit();
	//uic.Init(4, 6, mainWindow);
	 
	if (glewStatus != GLEW_OK) {
		LOG_ERROR("Bad GLEW Init.");
		//uic.Exit();
		glfwTerminate();
		return -1;
	}

	c.Init();
	
	uic.Init(C_VERSION_MAJOR, C_VERSION_MINOR, mainWindow, c);

	// - Main Loop -----------------------------------------------------------------
	while (!glfwWindowShouldClose(mainWindow)) {
		c.GetInputRouter().BeginFrame();
		c.PreUpdate();
		glfwPollEvents();
		c.Update();
		uic.Update();
		glfwSwapBuffers(mainWindow);
		c.PostUpdate();
	}



	uic.Exit();
	c.Stop();
	c.Cleanup();
	//uic.Exit();

	glfwTerminate();

}









