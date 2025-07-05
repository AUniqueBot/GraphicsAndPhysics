
#include <pch.h>
#include <graphics/gfx_glfwCustomCallbackFunctions.h>
#include <arch/core.h>
#include <arch/resources/res_mesh.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



// these are coordinates in normalised window space 
glm::vec3 cubePos[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f)
};

enum IMAGE_CLAMP_BEHAVIOUR {
	REPEAT,
	MIRROR_REPEAT,
	TO_EDGE,
	TO_BORDER
};

enum FILTER_TYPE {
	LINEAR,
	NEAREST
};


struct Light {
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 specular;


	int type; // 0 == point, 1 == directional, 2 == spot, 3 == area.
};


unsigned LoadImage(std::string path, bool _hasAlpha, IMAGE_CLAMP_BEHAVIOUR _horizontal, IMAGE_CLAMP_BEHAVIOUR _vertical, FILTER_TYPE _fType) {
	// TODO - Retrofit this function to save as a class for asset management.

	unsigned id{};
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	// set the texture wrapping/filtering options (on currently bound texture)
	GLenum hwrap{};
	GLenum vwrap{};
	for (int i{}; i < 2; ++i) {
		GLenum* setting = (i == 0) ? &hwrap : &vwrap;
		IMAGE_CLAMP_BEHAVIOUR* specifier = (i == 0) ? &_horizontal : &_vertical;

		switch (*specifier) {
		case IMAGE_CLAMP_BEHAVIOUR::REPEAT:
			*setting = GL_REPEAT;
			break;
		case IMAGE_CLAMP_BEHAVIOUR::MIRROR_REPEAT:
			*setting = GL_MIRRORED_REPEAT;
			break;
		case IMAGE_CLAMP_BEHAVIOUR::TO_EDGE:
			*setting = GL_CLAMP_TO_EDGE;
			break;
		case IMAGE_CLAMP_BEHAVIOUR::TO_BORDER:
			*setting = GL_CLAMP_TO_BORDER;
			break;
		}

		
	}
	GLint filter = _fType == FILTER_TYPE::LINEAR ? GL_LINEAR : GL_NEAREST;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, hwrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vwrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	// load and generate the texture
	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	GLint format = _hasAlpha ? GL_RGBA : GL_RGB;

	if (!data) {
		std::cout << path << ": Failed to load texture" << std::endl;
		stbi_image_free(data);
		return id;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	std::cout << path << ": Load OK" << std::endl;
	stbi_image_free(data);

	return id;
}



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

	Core c;


	// - GLFW Initialisation ------------------------------------------------------
	glfwInit();

	// setting up opengl version (opengl 4.)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	// tbh mate, idk what this does.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// create a window
	GLFWwindow* mainWindow = glfwCreateWindow(1920, 1080, "BWOAH", nullptr, nullptr);
	c.m_window = mainWindow;


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
	unsigned int tex1{}, tex2{}, tex3{};
	unsigned int diffuse{}, specular{};

	// LoadImages(tex1, tex2);
	tex1		= LoadImage("Assets/Images/container.jpg", false, TO_BORDER, TO_BORDER, LINEAR);
	tex2		= LoadImage("Assets/Images/TestImage.png", true, REPEAT, REPEAT, NEAREST);
	tex3		= LoadImage("Assets/Images/awesomeface.png", true, REPEAT, REPEAT, NEAREST);

	diffuse		= LoadImage("Assets/Images/container2.png", true, REPEAT, REPEAT, NEAREST);
	specular	= LoadImage("Assets/Images/container2_specular.png", true, REPEAT, REPEAT, NEAREST);


	// - callbacks -------------------
	glfwSetFramebufferSizeCallback(mainWindow, glfw_resizeCallback);
	glfwSetCursorPosCallback(mainWindow, MouseCallback);
	glfwSetMouseButtonCallback(mainWindow, MouseButtonCallback);


	/*
	For reference
		- VBO = What the vertex data is
		- VAO = How to interpret a single vertex
		- EBO = Which vertices to use to make geometry
	*/

	// - note that we're using a mesh here. --------------------


	// use the shader program here.
	c.Init();

#pragma region // - shader compilation ---------
	// - shader compilation ---------
	// ----------------------------------------------------------------------------






#pragma endregion


	// transforms

	// typical conversion is model mtx -> view mtx -> projection mtx
	// coordinates are transformed 
	// local -[model]-> world -[view]-> view -[projection]-> clip
	glm::mat4 pos, rot, scl{ 1.f };
	glm::vec3 lightPos{ 2, 5, 3 };
	glm::vec3 lightCol	{ 0.7f };
	glm::vec3 lightSpec	{ 1.0f };


	// - Main Loop -----------------------------------------------------------------
	while (!glfwWindowShouldClose(mainWindow)) {


	
		c.Update();

		// - Swapping Buffers ---------------
		glfwSwapBuffers(mainWindow);
		glfwPollEvents();


	}

	c.Stop();
	c.Cleanup();

	glfwTerminate();

}

