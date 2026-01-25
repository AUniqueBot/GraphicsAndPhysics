#include <arch/resources/res_mesh_presets/res_mesh_plane.h>


void Plane::Init() {
	UpdateVertexData();
}



void Plane::UpdateVertexData() {
	ClearMeshInformation();

	glm::vec3 startingPosition{ m_dimensions.x, 0, m_dimensions.y };
	startingPosition.x *= -0.5f;
	startingPosition.y *= 0.5f;

	unsigned pointsU{ static_cast<unsigned>(m_subdivisions.x) + 2 };
	unsigned pointsV{ static_cast<unsigned>(m_subdivisions.y) + 2 };

	const float widthDelta			{ m_dimensions.x / (pointsU - 1) };
	const float heightDelta			{ m_dimensions.y / (pointsV - 1) };
	m_vertexNormals.assign(static_cast<size_t>(pointsU * pointsV), { 0.f, 1.f, 0.f });
	// normals for faces are exactly the same and should be

	for (unsigned v{}; v < pointsV; ++v) {
		for (unsigned u{}; u < pointsU; ++u) {
			glm::vec3 vtxPos{startingPosition};
			vtxPos.x += static_cast<float>(u) * widthDelta;
			// vtxPos.y = 0;
			vtxPos.z -= static_cast<float>(v) * heightDelta;
			m_vertexPositions.push_back(vtxPos);

			if (u < pointsU - 1 && v < pointsV - 1) {
				unsigned i0	{ u + pointsU * v };
				unsigned i1	{ i0 + 1 };
				unsigned i2	{ i0 + pointsU };
				unsigned i3	{ i2 + 1 };

				m_indices.push_back({ i0, i1, i2 });
				m_indices.push_back({ i1, i3, i2 });
			}

		}
	}

}