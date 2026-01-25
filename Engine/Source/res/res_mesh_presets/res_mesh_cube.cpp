#include <arch/resources/res_mesh_presets/res_mesh_cube.h>

// - statics -------------------------------------------

//static float positionData[] = {
//	// x y z.
//
//	// back face
//	-0.5f, -0.5f, -0.5f,
//	 0.5f, -0.5f, -0.5f,
//	-0.5f,  0.5f, -0.5f,
//	 0.5f,  0.5f, -0.5f,
//	 // front face
//	 -0.5f, -0.5f,  0.5f,
//	 0.5f, -0.5f,  0.5f,
//	 -0.5f,  0.5f,  0.5f,
//	 0.5f,  0.5f,  0.5f,
//	 // left face
//	 -0.5f, -0.5f, -0.5f,
//	 -0.5f, -0.5f,  0.5f,
//	 -0.5f,  0.5f, -0.5f,
//	 -0.5f,  0.5f,  0.5f,
//	 // right face
//	 0.5f, -0.5f, -0.5f,
//	 0.5f, -0.5f,  0.5f,
//	 0.5f,  0.5f, -0.5f,
//	 0.5f,  0.5f,  0.5f,
//	 // bottom face
//	 -0.5f, -0.5f, -0.5f,
//	 0.5f, -0.5f, -0.5f,
//	 -0.5f, -0.5f,  0.5f,
//	 0.5f, -0.5f,  0.5f,
//	 // top face
//	 -0.5f,  0.5f, -0.5f,
//	 0.5f,  0.5f, -0.5f,
//	 -0.5f,  0.5f,  0.5f,
//	 0.5f,  0.5f,  0.5f
//};
//
//static float normalData[] = {
//	// x y z.
//
//	// back face
//	0.0f, 0.0f, -1.0f,
//	0.0f, 0.0f, -1.0f,
//	0.0f, 0.0f, -1.0f,
//	0.0f, 0.0f, -1.0f,
//	// front face
//	0.0f, 0.0f,  1.0f,
//	0.0f, 0.0f,  1.0f,
//	0.0f, 0.0f,  1.0f,
//	0.0f, 0.0f,  1.0f,
//	// left face
//	-1.0f, 0.0f, 0.0f,
//	-1.0f, 0.0f, 0.0f,
//	-1.0f, 0.0f, 0.0f,
//	-1.0f, 0.0f, 0.0f,
//	// right face
//	1.0f, 0.0f, 0.0f,
//	1.0f, 0.0f, 0.0f,
//	1.0f, 0.0f, 0.0f,
//	1.0f, 0.0f, 0.0f,
//	// bottom face
//	0.0f, -1.0f, 0.0f,
//	0.0f, -1.0f, 0.0f,
//	0.0f, -1.0f, 0.0f,
//	0.0f, -1.0f, 0.0f,
//	// top face
//	0.0f,  1.0f, 0.0f,
//	0.0f,  1.0f, 0.0f,
//	0.0f,  1.0f, 0.0f,
//	0.0f,  1.0f, 0.0f
//};
//
//static float uvData[] = {
//	0.0f, 0.0f,
//	1.0f, 0.0f,
//	0.0f, 1.0f,
//	1.0f, 1.0f,
//	0.0f, 0.0f,
//	1.0f, 0.0f,
//	0.0f, 1.0f,
//	1.0f, 1.0f,
//	0.0f, 0.0f,
//	1.0f, 0.0f,
//	0.0f, 1.0f,
//	1.0f, 1.0f,
//	0.0f, 0.0f,
//	0.0f, 1.0f,
//	1.0f, 0.0f,
//	1.0f, 1.0f,
//	0.0f, 0.0f,
//	1.0f, 0.0f,
//	0.0f, 1.0f,
//	1.0f, 1.0f,
//	0.0f, 0.0f,
//	1.0f, 0.0f,
//	0.0f, 1.0f,
//	1.0f, 1.0f
//};
//
//static glm::uvec3 idxList[] = { // note that we start from 0!
//	glm::uvec3(0, 1, 3), // first triangle
//	glm::uvec3(0, 2, 3), // second triangle
//
//	glm::uvec3(4, 5, 7),
//	glm::uvec3(4, 6, 7),
//
//	glm::uvec3(8, 9, 11),
//	glm::uvec3(8, 10, 11),
//
//	glm::uvec3(12, 13, 15),
//	glm::uvec3(12, 14, 15),
//
//	glm::uvec3(16, 17, 19),
//	glm::uvec3(16, 18, 19),
//
//	glm::uvec3(20, 21, 23),
//	glm::uvec3(20, 22, 23)
//};

//const size_t Cube::GetVertexDataSize() const {
//	return sizeof(positionData);
//}
//
//const float* Cube::GetVertexData() const {
//	return positionData;
//}
//
//const size_t Cube::GetNormalDataSize() const {
//	return sizeof(normalData);
//}
//
//const float* Cube::GetNormalData() const {
//	return normalData;
//}
//
//const unsigned Cube::GetUVCount() const {
//	return 1;
//}
//
//const size_t Cube::GetUVDataSize(unsigned _index) const {
//	(void)_index;
//	return sizeof(uvData);
//}
//
//const float* Cube::GetUVData(unsigned _index) const {
//	return uvData;
//	//return nullptr;
//}
//
//const size_t Cube::GetIndexDataSize() const {
//	return sizeof(idxList);
//}
//
//const glm::uvec3* Cube::GetIndexData() const {
//	return idxList;
//}
//
//const unsigned Cube::GetIndexDataCount() const {
//	return sizeof(idxList) / sizeof(idxList[0]);
//}


void Cube::Init() {
	// init vertex positions
	// m_vertexPositions.assign(, );
	//size_t vtxCount = sizeof(positionData) / (sizeof(positionData[0]) * 3);
	//AssignVertexPositions(positionData, vtxCount);
	//AssignVertexNormals(normalData, vtxCount);
	//AssignIndices(idxList, sizeof(idxList) / sizeof(unsigned));

	UpdateVertexData();
}



void Cube::UpdateVertexData() {
	ClearMeshInformation();
	// forward/backward.
	for (unsigned sign{}; sign < 2; ++sign) {
		for (unsigned side{}; side < 3; ++side) {
			// 0 = x (lr)
			// 1 = y (fb)
			// 2 = z (ud)
		
		
			// do 2 sides at once.
			glm::vec3 startOffset	{ m_dimensions.x, m_dimensions.y, m_dimensions.z };
			startOffset *= -0.5f;
			
			glm::vec3 vertexOffsetWidth			{};
			glm::vec3 vertexOffsetHeight		{};
			glm::vec3 vertexNormal				{};
			unsigned segmentU					{};
			unsigned segmentV					{};
			bool flipped						{ sign == 1 };
			

			switch (side) {
			case 0:
				vertexNormal = { 1, 0, 0 };
				if (!flipped) startOffset.x *= -1;
				segmentU = (m_subdivisions.y + 2);
				segmentV = (m_subdivisions.z + 2);
				vertexOffsetWidth.y = m_dimensions.y / (segmentU - 1);
				vertexOffsetHeight.z = m_dimensions.z / (segmentV - 1);
				break;
		
			case 1:
				vertexNormal = { 0, 1, 0 };
				if (!flipped) startOffset.y *= -1;
				segmentU = (m_subdivisions.x + 2);
				segmentV = (m_subdivisions.z + 2);
				vertexOffsetWidth.x = m_dimensions.x / (segmentU - 1);
				vertexOffsetHeight.z = m_dimensions.z / (segmentV - 1);
				break;

			case 2:
				vertexNormal = { 0, 0, 1 };
				if (!flipped) startOffset.z *= -1;
				segmentU = (m_subdivisions.x + 2);
				segmentV = (m_subdivisions.y + 2);
				vertexOffsetWidth.x = m_dimensions.x / (segmentU - 1);
				vertexOffsetHeight.y = m_dimensions.y / (segmentV - 1);
				break;
			}
			if (flipped) vertexNormal *= -1;

			unsigned faceNo		{ (side + 1) * (sign + 1) };
			unsigned baseIndex	{ static_cast<unsigned>(m_vertexPositions.size()) };

			// row priority
			for (int v{}; v < segmentV; ++v) {
				for (int u{}; u < segmentU; ++u) {
					m_vertexPositions.push_back({ 
						startOffset 
						+ (vertexOffsetWidth * static_cast<float>(u)) 
						+ (vertexOffsetHeight * static_cast<float>(v))
						});
					m_vertexNormals.push_back(vertexNormal);
					



					if (v < segmentV - 1 && u < segmentU - 1) {
						/*
						   0		  1
							+--------+
							|		 |
							|		 |
							|		 |
							|		 |
							|		 |
							+--------+
						   2		  3
						*/


						unsigned i0 = baseIndex + u + segmentU * v;
						unsigned i1 = i0 + 1;
						unsigned i2 = i0 + segmentU;
						unsigned i3 = i2 + 1;
						
						if (flipped) {
							m_indices.push_back({ i0, i2, i1 });
							m_indices.push_back({ i1, i2, i3 });
						}
						else {
							m_indices.push_back({ i0, i1, i2 });
							m_indices.push_back({ i1, i3, i2 });
						}
					}
				}
			}
		}
	}
 }
