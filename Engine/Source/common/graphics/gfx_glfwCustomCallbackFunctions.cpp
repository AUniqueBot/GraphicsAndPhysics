
#include <pch.h>
#include <graphics/gfx_glfwCustomCallbackFunctions.h>


void glfw_resizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}