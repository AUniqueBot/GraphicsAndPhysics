#include <arch/resources/res_mesh_presets/res_mesh_cube.h>

// - statics -------------------------------------------

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


void Cube::Init() {
	// init vertex positions
//m_vertexPositions.assign(, );

	size_t vtxCount = sizeof(positionData) / (sizeof(positionData[0]) * 3);
	AssignVertexPositions(positionData, vtxCount);
	AssignVertexNormals(normalData, vtxCount);
	AssignIndices(idxList, sizeof(idxList) / sizeof(unsigned));
}
