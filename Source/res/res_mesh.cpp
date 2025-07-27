#include <pch.h>
#include <arch/resources/res_mesh.h>

// - statics -------------------------------------------

static float vtxData[] = {
	// positions		// normal			// uv

	// x y z.

	// back face
	-0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,    0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,    1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f,    0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f,    1.0f, 1.0f,

	-0.5f, -0.5f,  0.5f,    0.0f, 0.0f,  1.0f,    0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,    0.0f, 0.0f,  1.0f,    1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,    0.0f, 0.0f,  1.0f,    0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,    0.0f, 0.0f,  1.0f,    1.0f, 1.0f,
    
	-0.5f, -0.5f, -0.5f,    -1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,    -1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,    -1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,    -1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
	
	 0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,     1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,     1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,     1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
	
	-0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f,    0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f,    1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,    0.0f, -1.0f, 0.0f,    0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,    0.0f, -1.0f, 0.0f,    1.0f, 1.0f,
	
	-0.5f,  0.5f, -0.5f,    0.0f,  1.0f, 0.0f,    0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,    0.0f,  1.0f, 0.0f,    1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,    0.0f,  1.0f, 0.0f,    0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,    0.0f,  1.0f, 0.0f,    1.0f, 1.0f  
};

static float positionData[] = {
	// x y z.

	// back face
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	// front face
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	// left face
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f,
	// right face
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f,  0.5f,  0.5f,
	// bottom face
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	// top face
	-0.5f,  0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f,
	0.5f,  0.5f,  0.5f
};

static float normalData[] = {
	// x y z.

	// back face
	0.0f, 0.0f, -1.0f,  
	0.0f, 0.0f, -1.0f,  
	0.0f, 0.0f, -1.0f,  
	0.0f, 0.0f, -1.0f,  
	// front face
	0.0f, 0.0f,  1.0f,  
	0.0f, 0.0f,  1.0f,  
	0.0f, 0.0f,  1.0f,  
	0.0f, 0.0f,  1.0f,  
	// left face
	-1.0f, 0.0f, 0.0f,  
	-1.0f, 0.0f, 0.0f,  
	-1.0f, 0.0f, 0.0f,  
	-1.0f, 0.0f, 0.0f,  
	// right face
	1.0f, 0.0f, 0.0f,   
	1.0f, 0.0f, 0.0f,   
	1.0f, 0.0f, 0.0f,   
	1.0f, 0.0f, 0.0f,   
	// bottom face
	0.0f, -1.0f, 0.0f,  
	0.0f, -1.0f, 0.0f,  
	0.0f, -1.0f, 0.0f,  
	0.0f, -1.0f, 0.0f,  
	// top face
	0.0f,  1.0f, 0.0f,  
	0.0f,  1.0f, 0.0f,  
	0.0f,  1.0f, 0.0f,  
	0.0f,  1.0f, 0.0f  
};

static float uvData[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f
};

static unsigned int idxList[] = { // note that we start from 0!
	0, 1, 3, // first triangle
	0, 2, 3, // second triangle

	4, 5, 7,
	4, 6, 7,

	8, 9, 11,
	8, 10, 11,

	12, 13, 15,
	12, 14, 15,

	16, 17, 19,
	16, 18, 19,

	20, 21, 23,
	20, 22, 23
};

// - class methods -------------------------------------

void Mesh::Load() {

	Init();

}

void Mesh::Init() {
	/*
		initialises a mesh.
	
	*/

	// generate buffers
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(3, m_vbo);
	glGenBuffers(1, &m_ebo);

	

	UseVAO();

	// filling vertex data
	
	// position data
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]); // replace with vertex buffer by assimp.
	glBufferData(GL_ARRAY_BUFFER, sizeof(positionData), positionData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0); // at position 0

	// normal data
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normalData), normalData, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1); // at position 1

	// uv data
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvData), uvData, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2); // at position 2

	// filling vertex to face data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idxList), idxList, GL_STATIC_DRAW);

	// unbind 
	glBindVertexArray(0);
	m_indexCount = sizeof(idxList) / sizeof(unsigned);
	
}



void Mesh::UseVAO() const {
	glBindVertexArray(m_vao);
}

