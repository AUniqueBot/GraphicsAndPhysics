#version 330 core

// for now, start with the basics and only focus on position.
layout (location = 0) in vec3 aPos;


uniform vec4 color;



void main() {
	// Set the position of the vertex
	gl_Position = vec4(aPos, 1.0);
	// Set the color of the vertex
	gl_FrontColor = color;

}


/*
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
*/