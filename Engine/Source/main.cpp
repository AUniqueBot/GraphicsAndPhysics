
#include <pch.h>
#include <graphics/gfx_glfwCustomCallbackFunctions.h>
#include <arch/core.h>
#include <arch/resources/res_mesh.h>

#include <UI_Core.h>

//#include <imgui/imgui.h>
//
//#include <imgui/imgui_impl_glfw.h>
//#include <imgui/imgui_impl_opengl3.h>



// - main -----------------------------------------------------------------------------------------------------
int main() {
	Core& c = Core::GetInstance();
	UI_Core uic {};
	// core to also initialise the timer
	// - GLFW Initialisation ------------------------------------------------------
	
	// abstract to window.
	glfwInit();
	// setting up opengl version (opengl 4.)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

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



	// - GLEW Initialisation -------------------------------------------------------
	GLenum glewStatus = glewInit();
	uic.Init(4, 6, mainWindow);
	
	if (glewStatus != GLEW_OK) {
		LOG_ERROR("Bad GLEW Init.");
		uic.Exit();
		glfwTerminate();
		return -1;
	}

	c.Init();


	// - Main Loop -----------------------------------------------------------------
	while (!glfwWindowShouldClose(mainWindow)) {
		
		glfwPollEvents();
		c.Update();
		uic.Update();
		glfwSwapBuffers(mainWindow);

	}




	c.Stop();
	c.Cleanup();
	uic.Exit();

	glfwTerminate();

}

