#include <arch/resources/res_mesh_presets/res_mesh_cube.h>



Cube::Cube(glm::vec3 _dimensions, glm::ivec3 _subdivisions) : 
	m_dimensions{ _dimensions }, m_subdivisions{ _subdivisions }
{
	m_name = "Cube";
	Init();
}

void Cube::Init() {

	UpdateVertexData();
}



void Cube::UpdateVertexData() {
	ClearMeshInformation();

	std::vector<glm::vec3> vertexPositions;
	std::vector<glm::vec3> vertexNormals;
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
			unsigned baseIndex	{ static_cast<unsigned>(vertexPositions.size()) };

			// row priority
			for (int v{}; v < segmentV; ++v) {
				for (int u{}; u < segmentU; ++u) {
					vertexPositions.push_back({ 
						startOffset 
						+ (vertexOffsetWidth * static_cast<float>(u)) 
						+ (vertexOffsetHeight * static_cast<float>(v))
						});
					vertexNormals.push_back(vertexNormal);
					



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
 
	SetData("position", reinterpret_cast<float*>(vertexPositions.data()), vertexPositions.size() * 3);
	SetData("normal", reinterpret_cast<float*>(vertexNormals.data()), vertexPositions.size() * 3);
}
