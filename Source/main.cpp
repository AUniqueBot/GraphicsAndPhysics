
#include <pch.h>
#include <graphics/gfx_glfwCustomCallbackFunctions.h>
#include <arch/core.h>
#include <arch/resources/res_mesh.h>






glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camFwd = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
bool mouseLook = false;

// - callbacks -----------------------------------------------------------------------------------------------

void MouseCallback(GLFWwindow* window, double xpos, double ypos) {

	if (!mouseLook) return;

	static bool firstMouse = true;
	static float lastX = 400.0f;
	static float lastY = 300.0f;

	static double yaw		{};
	static double pitch		{};

	float xoffset, yoffset;

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
		return; // no movement on first frame
	}

	xoffset = xpos - lastX;
	yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;
	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// Constrain pitch
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	// Convert to direction vector
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camFwd = glm::normalize(direction);
}
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		if (glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Lock and hide
			mouseLook = true;
		}
	}
}
void Input(GLFWwindow*& _window) {
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		mouseLook = false;
		glfwWindowShouldClose(_window);
		return;
	}

	// camera adjustmemts.
	const float cameraSpeed = 0.05f; // adjust accordingly
	if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
		camPos += cameraSpeed * camFwd;
	if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
		camPos -= cameraSpeed * camFwd;
	if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
		camPos -= glm::normalize(glm::cross(camFwd, camUp)) *
		cameraSpeed;
	if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
		camPos += glm::normalize(glm::cross(camFwd, camUp)) *
		cameraSpeed;
	if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camPos += glm::normalize(camUp) *
		cameraSpeed;
	if (glfwGetKey(_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camPos -= glm::normalize(camUp) *
		cameraSpeed;
}





// - main -----------------------------------------------------------------------------------------------------
int main() {

	Core& c = Core::GetInstance();
	// core to also initialise the timer
	// - GLFW Initialisation ------------------------------------------------------
	
	// abstract to window.
	glfwInit();
	// setting up opengl version (opengl 4.)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	// tbh mate, idk what this does.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// create a window
	GLFWwindow* mainWindow = glfwCreateWindow(1920, 1080, "BWOAH", nullptr, nullptr);
	c.SetWindow(mainWindow);


	// failed to create window
	if (!mainWindow) {
		std::cout << "Bad GLFW Init.\n";
		glfwTerminate();
		return -1;
	}


	glfwMakeContextCurrent(mainWindow);

	// - GLEW Initialisation -------------------------------------------------------
	GLenum glewStatus = glewInit();
	if (glewStatus != GLEW_OK) {
		std::cout << "Bad GLEW Init\n";
		glfwTerminate();
		return -1;
	}

	c.Init();


	// - Main Loop -----------------------------------------------------------------
	while (!glfwWindowShouldClose(mainWindow)) {
		c.Update();

		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
	}

	c.Stop();
	c.Cleanup();

	glfwTerminate();

}

