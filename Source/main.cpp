
#include <pch.h>
#include <graphics/gfx_glfwCustomCallbackFunctions.h>
#include <arch/core.h>



float vertexArray[]{
	0.5f, 0.5f, 0.0f, // top right
	0.5f, -0.5f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f, // bottom left
	-0.5f, 0.5f, 0.0f // top left
};

float texCoords[] = {
	1.0f, 1.0f, // lower-left corner
	1.0f, 0.0f, // lower-right corner
	0.0f, 0.0f, // top-center corner
	0.0f, 1.0f // top-center corner
};


unsigned int indices[] = { // note that we start from 0!
	0, 1, 3, // first triangle
	1, 2, 3 // second triangle
};

const char* vtxSrc = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform float offset;\n"
"out vec4 vtxPos;"
"void main() {\n"
"	gl_Position = vec4(aPos.x + offset, aPos.y, aPos.z, 1.0);\n"
"	vtxPos = gl_Position;\n"
"}\0";

// source code for fragment shader
const char* fragSrc =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vtxPos;\n"
"void main() {\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"	FragColor = vtxPos;\n"
"}\0";

int main() {


	Core c;

	c.Init();

	// - GLFW Initialisation ------------------------------------------------------
	glfwInit();

	// setting up opengl version (opengl 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	// tbh mate, idk what this does.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	

	// create a window
	GLFWwindow* mainWindow = glfwCreateWindow(1920, 1080, "Window Name", nullptr, nullptr);

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


	
	// - set up functions ------------
	 glfwSetFramebufferSizeCallback(mainWindow, glfw_resizeCallback);


	 // creation of VBOs and VAOs
	 unsigned VBO					{};		// 
	 unsigned VAO					{};		// 
	 unsigned EBO					{};		//

	 glGenBuffers(1, &VBO);					// VBO Gets assigned an ID of sorts
	 glBindBuffer(GL_ARRAY_BUFFER, VBO);	// binding ID to an array buffer
	 glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);	// filling array buffer data

	 glGenVertexArrays(1, &VAO);
	 glBindVertexArray(VAO);
	 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	 glEnableVertexAttribArray(0);

	 glGenBuffers(1, &EBO);

	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	 glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	 glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	 // use the shader program here.

	#pragma region // - shader compilation ---------
	 // - shader compilation ---------
	 // ----------------------------------------------------------------------------
	 int vtxShader = glCreateShader(GL_VERTEX_SHADER);	// assign this id to a vertex shader

	 glShaderSource(vtxShader, 1, &vtxSrc, NULL);
	 glCompileShader(vtxShader);

	 // a class for a shader would be a good idea?

	 // warning check
	 // status of thing
	 int glStatus{};
	 glGetShaderiv(vtxShader, GL_COMPILE_STATUS, &glStatus);
	 if (glStatus == GL_FALSE) {
		 char log[512];
		 glGetShaderInfoLog(vtxShader, 512, NULL, log);

		 std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << log << std::endl;

	 }

	 // fragment shader
	 int fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	 glShaderSource(fragShader, 1, &fragSrc, NULL);
	 glCompileShader(fragShader);

	 glGetShaderiv(fragShader, GL_COMPILE_STATUS, &glStatus);
	 if (glStatus == GL_FALSE) {
		 char log[512];
		 glGetShaderInfoLog(fragShader, 512, NULL, log);

		 std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << log << std::endl;
	 }


	 // creation and compiling the shader
	 unsigned prg = glCreateProgram();
	 glAttachShader(prg, vtxShader);
	 glAttachShader(prg, fragShader);
	 glLinkProgram(prg);

	 glGetShaderiv(prg, GL_LINK_STATUS, &glStatus);
	 if (glStatus == GL_FALSE) {
		 char log[512];
		 glGetShaderInfoLog(fragShader, 512, NULL, log);

		 std::cout << "ERROR::SHADER::LINK::COMPILATION_FAILED\n" << log << std::endl;
	 }

	 glDeleteShader(vtxShader);
	 glDeleteShader(fragShader);
	#pragma endregion


	 // set to wireframe
	 glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	// - Main Loop -----------------------------------------------------------------
	while (!glfwWindowShouldClose(mainWindow)) {

		c.Update();

		// clear colour
		glClear(GL_COLOR_BUFFER_BIT);
		// - stencil
		// - depth
		// - colour

		int uniformLoc = glGetUniformLocation(prg, "offset");
		glUniform1f(uniformLoc, 0.5);

		// input keys.
		GLFW_KEY_W;
		GLFW_KEY_S;
		GLFW_KEY_A;
		GLFW_KEY_D;

		// handle input here.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

		glUseProgram(prg);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		
		// - Swapping Buffers ---------------
		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
		
	
	}


	c.Stop();
	c.Cleanup();

	glfwTerminate();

}

