#include <pch.h>
#include <graphics/gfx_glfwCustomCallbackFunctions.h>
#include <arch/core.h>
#include <arch/resources/res_mesh.h>

#include <UI_Core.h>

//
//#include <imgui/imgui_impl_glfw.h>
//#include <imgui/imgui_impl_opengl3.h>
const unsigned C_VERSION_MAJOR = 4;
const unsigned C_VERSION_MINOR = 6;


// - main -----------------------------------------------------------------------------------------------------
int main() {
	Core& c = Core::GetInstance();

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
	UI_Core uic{};
	uic.Init(C_VERSION_MAJOR, C_VERSION_MINOR, mainWindow);

	// - Main Loop -----------------------------------------------------------------
	while (!glfwWindowShouldClose(mainWindow)) {
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

