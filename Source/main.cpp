
#include <pch.h>
#include <graphics/gfx_glfwCustomCallbackFunctions.h>
#include <arch/core.h>

// image testing.
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// these are coordinates in normalised window space 
float vertexArray[] = {
	// positions // colors // texture coords
	0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,			// top right
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,		// bottom right
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,		// bottom left
	-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f			// top left
};


// this index specifies triangles based on vertex index (0, 1, 3 & 1, 2, 3 are separate triangles)
unsigned int indices[] = { // note that we start from 0!
	0, 1, 3, // first triangle
	1, 2, 3 // second triangle
};

const char* vtxSrc =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aCol;\n"
"layout (location = 2) in vec2 aUv;\n"
"out vec4 vtxPos;\n"
"out vec3 aColour;\n"
"out vec2 oUv;\n"
"uniform float x_offset;\n"
"uniform float y_offset;\n"
"void main() {\n"
"	gl_Position = vec4(aPos.x + x_offset, aPos.y + y_offset, aPos.z, 1.0);\n"
"	aColour = aCol;\n"
"	oUv = aUv;\n"
"}\0";

// source code for fragment shader
const char* fragSrc =
"#version 330 core\n"
"in vec4 vtxPos;\n"
"in vec3 aColour;\n"
"in vec2 oUv;\n"
"out vec4 FragColor;\n"

"uniform sampler2D tex1;\n"
"uniform sampler2D tex2;\n"

"void main() {\n"
// "	FragColor = mix(texture(tex1, TexCoord), texture(tex2, TexCoord), 0.2);\n"
"	FragColor = mix(texture(tex2, oUv), texture(tex1, oUv), 0.2); \n"
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


	 /*
		For reference
			- VBO = What the vertex data is
			- VAO = How to interpret a single vertex
			- EBO = Which vertices to use to make geometry
	 */



	 // creation of VBOs and VAOs
	 unsigned VBO					{};		// 
	 unsigned VAO					{};		// 
	 unsigned EBO					{};		//

	 glGenBuffers(1, &VBO);					// VBO Gets assigned an ID of sorts
	 glBindBuffer(GL_ARRAY_BUFFER, VBO);	// binding ID to an array buffer
	 glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);	// filling array buffer data

	 glGenVertexArrays(1, &VAO);
	 glBindVertexArray(VAO);
	 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),(void*)(0)); // Pos
	 glEnableVertexAttribArray(0);
	 glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),(void*)(3 * sizeof(float)));	// Col
	 glEnableVertexAttribArray(1);
	 glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),(void*)(6 * sizeof(float))); // UV
	 glEnableVertexAttribArray(2);

	 glGenBuffers(1, &EBO);

	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	 glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	 // - image loading --------------
		 unsigned int texture1;
		 unsigned int texture2;
		 glGenTextures(1, &texture1);
		 glBindTexture(GL_TEXTURE_2D, texture1);

		 // set the texture wrapping/filtering options (on currently bound texture)
		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		 // load and generate the texture
		 int width, height, nrChannels;
		 unsigned char* data = stbi_load("Assets/Images/awesomeface.png", &width, &height,
			 &nrChannels, 0);
		 if (data)
		 {
			 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,GL_UNSIGNED_BYTE, data);
			 glGenerateMipmap(GL_TEXTURE_2D);
			 std::cout << "awesomeface.jpg: Load OK" << std::endl;
		 }
		 else
		 {
			 std::cout << "awesomeface.jpg: Failed to load texture" << std::endl;
		 }
		stbi_image_free(data);
		


		glGenTextures(1, &texture2);
		glBindTexture(GL_TEXTURE_2D, texture2);


		// set the texture wrapping/filtering options (on currently bound texture)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		data = stbi_load("Assets/Images/container.jpg", &width, &height,
			&nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			std::cout << "container.png: Load OK" << std::endl;
		}
		else
		{
			std::cout << "container.png: Failed to load texture" << std::endl;
		}

		stbi_image_free(data);

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

	 // tex binding.
	 // uses program, not the specific shader.
	 glUseProgram(prg);
	 glUniform1i(glGetUniformLocation(prg, "tex1"), 0);
	 glUniform1i(glGetUniformLocation(prg, "tex2"), 1);

	 // set to wireframe
	 glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	// - Main Loop -----------------------------------------------------------------
	while (!glfwWindowShouldClose(mainWindow)) {
		
		c.Update();

		// clear colour
		glClear(GL_COLOR_BUFFER_BIT);
		// - stencil
		// - depth
		// - colour


		// this specifies the uniform, "offset" in the shader to be 0.5
		// it returns an int (name) as the location specifier for the shader.
		// if it doesn't exist probably -1?
		int uniformLoc{}; 
		uniformLoc = glGetUniformLocation(prg, "x_offset");
		glUniform1f(uniformLoc, -0.2);
		uniformLoc = glGetUniformLocation(prg, "y_offset");
		glUniform1f(uniformLoc, 0.2);

		// input keys.
		GLFW_KEY_W;
		GLFW_KEY_S;
		GLFW_KEY_A;
		GLFW_KEY_D;




		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		
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

