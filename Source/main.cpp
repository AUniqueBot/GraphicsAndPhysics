
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


std::string GetRawText(std::string _pathToFile) {
	std::stringstream toRet;
	std::ifstream ifs{ _pathToFile };
	if (!ifs.good()) return std::string{};
	toRet << ifs.rdbuf();
	return toRet.str();
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
	c.Init();

	

	// - GLFW Initialisation ------------------------------------------------------
	glfwInit();

	// setting up opengl version (opengl 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	// tbh mate, idk what this does.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// create a window
	GLFWwindow* mainWindow = glfwCreateWindow(1920, 1080, "BWOAH", nullptr, nullptr);
	
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
	tex1 = LoadImage("Assets/Images/container.jpg", false, TO_BORDER, TO_BORDER, LINEAR);
	tex2 = LoadImage("Assets/Images/TestImage.png", true, REPEAT, REPEAT, NEAREST);
	tex3 = LoadImage("Assets/Images/awesomeface.png", true, REPEAT, REPEAT, NEAREST);

	diffuse = LoadImage("Assets/Images/container2.png", true, REPEAT, REPEAT, NEAREST);
	specular = LoadImage("Assets/Images/container2_specular.png", true, REPEAT, REPEAT, NEAREST);


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


	Mesh mesh;
	mesh.Init();


	// use the shader program here.

#pragma region // - shader compilation ---------
	// - shader compilation ---------
	// ----------------------------------------------------------------------------

	std::string srcS{};
	const char* src{};
	int glStatus{};


	// vertex shader
	int vtxShader = glCreateShader(GL_VERTEX_SHADER);	// assign this id to a vertex shader
	srcS = GetRawText("Assets/Shaders/vertexShader.vert");
	src = srcS.c_str();
	glShaderSource(vtxShader, 1, &src, NULL);
	glCompileShader(vtxShader);
	glGetShaderiv(vtxShader, GL_COMPILE_STATUS, &glStatus);

	if (glStatus == GL_FALSE) {
		char log[512];
		glGetShaderInfoLog(vtxShader, 512, NULL, log);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << log << std::endl;
	}

	// fragment shader
	int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	srcS = GetRawText("Assets/Shaders/phongShader.frag");
	src = srcS.c_str();
	glShaderSource(fragShader, 1, &src, NULL); // load shader source
	glCompileShader(fragShader); // compile the shaders.
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &glStatus);

	if (glStatus == GL_FALSE) {
		char log[512];
		glGetShaderInfoLog(fragShader, 512, NULL, log);
		std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << log << std::endl;
	}


	int flatShader = glCreateShader(GL_FRAGMENT_SHADER);
	srcS = GetRawText("Assets/Shaders/flatShader.frag");
	src = srcS.c_str();
	glShaderSource(flatShader, 1, &src, NULL); // load shader source
	glCompileShader(flatShader); // compile the shaders.
	glGetShaderiv(flatShader, GL_COMPILE_STATUS, &glStatus);

	if (glStatus == GL_FALSE) {
		char log[512];
		glGetShaderInfoLog(flatShader, 512, NULL, log);

		std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << log << std::endl;
	}


	int texShader = glCreateShader(GL_FRAGMENT_SHADER);
	srcS = GetRawText("Assets/Shaders/texturedFragShader.frag");
	src = srcS.c_str();
	glShaderSource(texShader, 1, &src, NULL); // load shader source
	glCompileShader(texShader); // compile the shaders.
	glGetShaderiv(texShader, GL_COMPILE_STATUS, &glStatus);
	if (glStatus == GL_FALSE) {
		char log[512];
		glGetShaderInfoLog(texShader, 512, NULL, log);
		std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << log << std::endl;
	}



	// creation and compiling the shader

	// can be done in update.
	unsigned prg = glCreateProgram();
	unsigned flatPrg = glCreateProgram();
	unsigned matPrg = glCreateProgram();

	glAttachShader(prg, fragShader);
	glAttachShader(prg, vtxShader);
	glLinkProgram(prg);

	glAttachShader(flatPrg, flatShader);
	glAttachShader(flatPrg, vtxShader);
	glLinkProgram(flatPrg);

	glAttachShader(matPrg, texShader);
	glAttachShader(matPrg, vtxShader);
	glLinkProgram(matPrg);



	glGetShaderiv(prg, GL_LINK_STATUS, &glStatus);
	if (glStatus == GL_FALSE) {
		char log[512];
		glGetShaderInfoLog(prg, 512, NULL, log);

		std::cout << "ERROR::SHADER::LINK::COMPILATION_FAILED\n" << log << std::endl;
	}

	glGetShaderiv(flatPrg, GL_LINK_STATUS, &glStatus);
	if (glStatus == GL_FALSE) {
		char log[512];
		glGetShaderInfoLog(flatPrg, 512, NULL, log);

		std::cout << "ERROR::SHADER::LINK::COMPILATION_FAILED\n" << log << std::endl;
	}

	glGetShaderiv(matPrg, GL_LINK_STATUS, &glStatus);
	if (glStatus == GL_FALSE) {
		char log[512];
		glGetShaderInfoLog(matPrg, 512, NULL, log);

		std::cout << "ERROR::SHADER::LINK::COMPILATION_FAILED\n" << log << std::endl;
	}


	// deletion can be done after compile.
	glDeleteShader(vtxShader);
	glDeleteShader(fragShader);
	glDeleteShader(flatShader);
	glDeleteShader(texShader);


#pragma endregion


	// transforms

	// typical conversion is model mtx -> view mtx -> projection mtx
	// coordinates are transformed 
	// local -[model]-> world -[view]-> view -[projection]-> clip


	// set to wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glm::mat4 pos, rot, scl{ 1.f };
	glm::vec3 lightPos{ 2, 5, 3 };
	glm::vec3 lightCol{ .96f, .56f, .05f };


	// - Main Loop -----------------------------------------------------------------
	while (!glfwWindowShouldClose(mainWindow)) {

		c.Update();

		Input(mainWindow);
		

		// bg colour.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.39f, 0.58f, 0.93f, 1.0f);


		float radius = 5.f;
		lightPos.x = sin(glfwGetTime()) * radius;;
		lightPos.y = sin(glfwGetTime() / 2) * radius;
		lightPos.z = cos(glfwGetTime()) * radius;;


		// set up the camera
		glm::mat4 viewMtx	{ 1.f };
		pos = glm::translate(
			glm::mat4{ 1.f }, 
			camPos
		);
		rot = glm::rotate(
			glm::mat4		{ 1.f },
			(float)glm::radians(25.f) * (float)sin(glfwGetTime()),
			glm::vec3(0.f, 1.f, 0.f)
		);
		viewMtx =  pos * rot * glm::mat4{ 1.f };
		viewMtx = glm::inverse(viewMtx); // camera's position are inversely applied to apply camera view


		glm::mat4 projectionMtx{};
		
		// projection matrix
		projectionMtx = glm::perspective(
			glm::radians(60.f), // vertical fov 
			16.f/9.f,					// CAMERA aspect ratio
			0.0001f,					// near clip plane
			1000.f							// far clip plane.
		);

		// note - viewport per camera.
		//

		// orthogonal
		//GLint vp[4]{};
		//glGetIntegerv(GL_VIEWPORT, vp);
		//projectionMtx = glm::ortho(
		//	0.0f, (float)vp[2],
		//	0.0f, (float)vp[3],
		//	0.0001f,
		//	10000.f
		//);

		// look at mtx - from to and up.
		//const float radius = 10.0f;
		//float camX = sin(glfwGetTime()) * radius;
		//float camZ = cos(glfwGetTime()) * radius;
		//camPos = glm::vec3(camX, 0.f, camZ);
		//viewMtx = glm::lookAt(
		//	camPos,
		//	glm::vec3(),
		//	glm::vec3(0, 1, 0)
		//);

		// lookat for camera control
		viewMtx = glm::lookAt(camPos, camPos + camFwd, camUp);



		// to use the orthographic view, use this
		/*
			projectionMtx = glm::ortho(
				0.f,				// min x 
				1280.f,				// max x
				0.f,				// min y
				720.f,				// max y
				// clip planes
				0.001f,
				1000.f
			);
		
		*/
		// tex binding.
		// select the current shader, and apply the shader.
		int currentPrg;
		currentPrg = matPrg;
		glUseProgram(currentPrg); // uses program, not the specific shader.

		glUniform1i(glGetUniformLocation(currentPrg, "surfaceMaterial.diffuse"), 0); // this function binds texture slots. ->> TEXTURE_0 -> the uniform "tex1"
		glUniform1i(glGetUniformLocation(currentPrg, "surfaceMaterial.specular"), 1);

		// activating textures
		glActiveTexture(GL_TEXTURE0); // select tex 0
		glBindTexture(GL_TEXTURE_2D, diffuse);
		glActiveTexture(GL_TEXTURE1); // select tex 1
		glBindTexture(GL_TEXTURE_2D, specular);

		// clear colour

		// - stencil
		// - depth
		// - colour
		int uniformLoc{};
		uniformLoc = glGetUniformLocation(currentPrg, "view");
		glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(viewMtx)); // constant
		uniformLoc = glGetUniformLocation(currentPrg, "projection");
		glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(projectionMtx)); // constant
		uniformLoc = glGetUniformLocation(currentPrg, "viewPos");
		glUniform3fv(uniformLoc, 1, glm::value_ptr(pos));

		uniformLoc = glGetUniformLocation(currentPrg, "lightCol");
		glUniform3fv(uniformLoc, 1, glm::value_ptr(lightCol));
		uniformLoc = glGetUniformLocation(currentPrg, "lightPos");
		glUniform3fv(uniformLoc, 1, glm::value_ptr(lightPos));
		uniformLoc = glGetUniformLocation(currentPrg, "viewPos");
		glUniform3fv(uniformLoc, 1, glm::value_ptr(camPos));
		uniformLoc = glGetUniformLocation(currentPrg, "time");
		glUniform1f(uniformLoc, glfwGetTime());

		mesh.UseVAO();
		glm::mat4 objMat{ 1 };




		int cubeCount{ sizeof(cubePos) / sizeof(glm::vec3) };
		for (int i{}; i < 1; ++i) {

			// object matrix
			glm::mat4 objMat{ 1 };
			pos = glm::translate(objMat, cubePos[i]);
			rot = glm::scale(objMat, glm::vec3(5));
			scl = glm::rotate(objMat, 0.f, glm::normalize(glm::vec3(1.f, 1.f, 0.f)));

			// glm works in column major first (wtv that means), so the order is reversed.
			objMat = pos * rot * scl;

		

			// this specifies the uniform, "offset" in the shader to be 0.5
			// it returns an int (name) as the location specifier for the shader.
			// if it doesn't exist probably -1?
			uniformLoc = glGetUniformLocation(currentPrg, "trs");
			glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(objMat));




			// possible
			/*
				for (auto& vbo : ListOfObjs) {
					glBindVertexArray(vbo.vao);
					glDrawElements(GL_TRIANGLES, vbo.VtxCount, GL_UNSIGNED_INT, 0);
				}
				// to unbind
				glBindVertexArray(0);



				// maybe.


				// alternatively, everything in 1 shader is connected in 1 ebo, vao, vbo.

			

			*/

		
			glDrawElements(GL_TRIANGLES, mesh.IndexCount(), GL_UNSIGNED_INT, 0);
		}

		currentPrg = flatPrg;
		glUseProgram(currentPrg);
		pos = glm::translate(objMat, lightPos);
		rot = glm::scale(objMat, glm::vec3(1));
		scl = glm::rotate(objMat, 0.f, glm::vec3{ 1.f, 0.f, 0.f });
		objMat = pos * rot * scl;

		uniformLoc = glGetUniformLocation(currentPrg, "colour");
		glUniform4fv(uniformLoc, 1, glm::value_ptr(glm::vec4{1, 1, 1, 1})); // flat white cube
		uniformLoc = glGetUniformLocation(currentPrg, "trs");
		glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(objMat));
		uniformLoc = glGetUniformLocation(currentPrg, "view");
		glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(viewMtx)); // constant
		uniformLoc = glGetUniformLocation(currentPrg, "projection");
		glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(projectionMtx)); // constant
		uniformLoc = glGetUniformLocation(currentPrg, "viewPos");
		glUniform3fv(uniformLoc, 1, glm::value_ptr(pos));



		glDrawElements(GL_TRIANGLES, mesh.IndexCount(), GL_UNSIGNED_INT, 0);


		// unbind vertices.
		glBindVertexArray(0);
		// - Swapping Buffers ---------------
		glfwSwapBuffers(mainWindow);
		glfwPollEvents();


	}

	c.Stop();
	c.Cleanup();

	glfwTerminate();

}

