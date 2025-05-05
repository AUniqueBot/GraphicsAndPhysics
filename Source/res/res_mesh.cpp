#include <pch.h>
#include <arch/resources/res_mesh.h>





static float vtxData[] = {
	// positions		// normal			// uv

	// x y z.

	// back face
	-0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,    0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,    1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f,    0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f,    1.0f, 1.0f,
	 // front face
	 -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,  1.0f,    0.0f, 0.0f,
	  0.5f, -0.5f,  0.5f,    0.0f, 0.0f,  1.0f,    1.0f, 0.0f,
	 -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,  1.0f,    0.0f, 1.0f,
	  0.5f,  0.5f,  0.5f,    0.0f, 0.0f,  1.0f,    1.0f, 1.0f,
	  // left face
	  -0.5f, -0.5f, -0.5f,    -1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
	  -0.5f, -0.5f,  0.5f,    -1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
	  -0.5f,  0.5f, -0.5f,    -1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
	  -0.5f,  0.5f,  0.5f,    -1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
	  // right face
	   0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
	   0.5f, -0.5f,  0.5f,     1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
	   0.5f,  0.5f, -0.5f,     1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
	   0.5f,  0.5f,  0.5f,     1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
	   // bottom face
	  -0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f,    0.0f, 0.0f,
	   0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f,    1.0f, 0.0f,
	  -0.5f, -0.5f,  0.5f,    0.0f, -1.0f, 0.0f,    0.0f, 1.0f,
	   0.5f, -0.5f,  0.5f,    0.0f, -1.0f, 0.0f,    1.0f, 1.0f,
	   // top face
	  -0.5f,  0.5f, -0.5f,    0.0f,  1.0f, 0.0f,    0.0f, 0.0f,
	   0.5f,  0.5f, -0.5f,    0.0f,  1.0f, 0.0f,    1.0f, 0.0f,
	  -0.5f,  0.5f,  0.5f,    0.0f,  1.0f, 0.0f,    0.0f, 1.0f,
	   0.5f,  0.5f,  0.5f,    0.0f,  1.0f, 0.0f,    1.0f, 1.0f

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

void Mesh::Load() {
	// for now we're using an AoS structure.

	//



	Init();

}

void Mesh::Init() {



	// generate buffers
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	
	UseVAO();

	// filling vertex data
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtxData), vtxData, GL_STATIC_DRAW);


	// filling vertex to face data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idxList), idxList, GL_STATIC_DRAW);

	// defining vertex attributes.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));					// Pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // UV
	glEnableVertexAttribArray(2);

	// unbind 
	glBindVertexArray(0);
	m_indexCount = sizeof(idxList) / sizeof(unsigned);
	
}



void Mesh::UseVAO() {
	glBindVertexArray(m_vao);
}
