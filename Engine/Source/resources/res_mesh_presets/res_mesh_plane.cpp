#include <arch/resources/res_mesh_presets/res_mesh_plane.h>


Plane::Plane(PlaneCreationProps _props) :
	m_dimensions { _props.dimensions }, m_subdivisions { _props.subdivisions } {
	m_name = "Plane";
	
	Init();
}

void Plane::Init() {
	SetSubmeshCount(1);
	AddSubmesh(Submesh());
	UpdateVertexData();
}


void Plane::SetXDimensions(float _dims) {
	if (m_dimensions.x == _dims) return;
	m_dimensions.x = _dims;

}
void Plane::SetYDimensions(float _dims) {
	if (m_dimensions.y == _dims) return;
	m_dimensions.y = _dims;
}
void Plane::SetDimensions(glm::vec2 _dims) {
	if (m_dimensions == _dims) return;
	m_dimensions = _dims;
}

const float& Plane::GetXDimensions() const { 
	return m_dimensions.x;
}
const float& Plane::GetYDimensions() const {
	return m_dimensions.y;
}
const glm::vec2& Plane::GetDimensions() const {
	return m_dimensions;
}

void Plane::SetXSubdivisions(int _dims) {
	if (m_subdivisions.x == _dims) return;
	m_subdivisions.x = _dims;

}
void Plane::SetYSubdivisions(int _dims) {
	if (m_subdivisions.y == _dims) return;
	m_subdivisions.y = _dims;

}
void Plane::SetSubdivisions(glm::ivec2 _dims) {
	if (m_subdivisions == _dims) return;
	m_subdivisions = _dims;
}

const int& Plane::GetXSubdivisions() const {
	return m_subdivisions.x;
}
const int& Plane::GetYSubdivisions() const {
	return m_subdivisions.y;
}
const glm::ivec2& Plane::GetSubdivisions() const {
	return m_subdivisions;
}


void Plane::UpdateVertexData() {

	ClearMeshInformation();
	std::vector<glm::vec3> vertexPositions;
	std::vector<glm::vec3> vertexNormals;
	glm::vec3 startingPosition{ m_dimensions.x, 0, m_dimensions.y };
	startingPosition.x *= -0.5f;
	startingPosition.z *= 0.5f;

	unsigned pointsU{ static_cast<unsigned>(m_subdivisions.x) + 2 };
	unsigned pointsV{ static_cast<unsigned>(m_subdivisions.y) + 2 };

	const float widthDelta			{ m_dimensions.x / (pointsU - 1) };
	const float heightDelta			{ m_dimensions.y / (pointsV - 1) };
	vertexNormals.assign(static_cast<size_t>(pointsU * pointsV), { 0.f, 1.f, 0.f });
	// normals for faces are exactly the same and should be

	for (unsigned v{}; v < pointsV; ++v) {
		for (unsigned u{}; u < pointsU; ++u) {
			glm::vec3 vtxPos{startingPosition};
			vtxPos.x += static_cast<float>(u) * widthDelta;
			// vtxPos.y = 0;
			vtxPos.z -= static_cast<float>(v) * heightDelta;
			vertexPositions.push_back(vtxPos);

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

	// move to submesh.
	SetData<glm::vec3>("position", vertexPositions.data(), vertexPositions.size());
	SetData<glm::vec3>("normal", vertexNormals.data(), vertexPositions.size());

	// - new system ----
	Submesh& submesh = m_submeshList[0];
	submesh.ClearSubmeshInformation();
	submesh.SetVertexCount(vertexPositions.size());
	submesh.SetData<glm::vec3>("position", vertexPositions.data(), vertexPositions.size());
	submesh.SetData<glm::vec3>("normal", vertexNormals.data(), vertexPositions.size());
}