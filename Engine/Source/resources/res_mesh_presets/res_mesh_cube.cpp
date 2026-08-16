#include <arch/resources/res_mesh_presets/res_mesh_cube.h>

CubeRes::CubeRes(CubeCreationProps _props) : 
	m_dimensions{ _props.dimensions }, m_subdivisions{ _props.subdivisions }
{
	m_name = "Cube";
}

void CubeRes::Init() {
	SetSubmeshCount(1);
	AddSubmesh(Submesh());
	UpdateVertexData();
}

void CubeRes::SetXDimensions(float _dims) {
	if (m_dimensions.x == _dims) return;
	m_dimensions.x = _dims;
	UpdateVertexData();
}

void CubeRes::SetYDimensions(float _dims) {
	if (m_dimensions.y == _dims) return;
	m_dimensions.y = _dims;
	UpdateVertexData();
}

void CubeRes::SetZDimensions(float _dims) {
	if (m_dimensions.z == _dims) return;
	m_dimensions.z = _dims;
	UpdateVertexData();
}

void CubeRes::SetDimensions(glm::vec3 _dims) {
	if (m_dimensions == _dims) return;
	m_dimensions = _dims;
	UpdateVertexData();
}


const float& CubeRes::GetXDimensions() const {
	return m_dimensions.x;
}

const float& CubeRes::GetYDimensions() const {
	return m_dimensions.y;
}

const float& CubeRes::GetZDimensions() const {
	return m_dimensions.z;
}

const glm::vec3& CubeRes::GetDimensions() const {
	return m_dimensions;
}


// -----------------------------------------------------------
void CubeRes::SetXSubdivisions(int _dims) {
	if (m_subdivisions.x == _dims) return;
	m_subdivisions.x = _dims;
	UpdateVertexData();
}

void CubeRes::SetYSubdivisions(int _dims) {
	if (m_subdivisions.y == _dims) return;
	m_subdivisions.y = _dims;
	UpdateVertexData();
}

void CubeRes::SetZSubdivisions(int _dims) {
	if (m_subdivisions.z == _dims) return;
	m_subdivisions.z = _dims;
	UpdateVertexData();
}

void CubeRes::SetSubdivisions(glm::ivec3 _dims) {
	if (m_subdivisions == _dims) return;
	m_subdivisions = _dims;
	UpdateVertexData();
}

const int& CubeRes::GetXSubdivisions() const {
	return m_subdivisions.x;
}

const int& CubeRes::GetYSubdivisions() const {
	return m_subdivisions.y;
}

const int& CubeRes::GetZSubdivisions() const{
	return m_subdivisions.z;
}

const glm::ivec3& CubeRes::GetSubdivisions() const {
	return m_subdivisions;
}





// -----------------------------------------------------------
void CubeRes::UpdateVertexData() {
	ClearMeshInformation();

	std::vector<glm::vec3> vertexPositions;
	std::vector<glm::vec3> vertexNormals;
	std::vector<glm::uvec3> indices;
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
			for (int v{}; v < static_cast<int>(segmentV); ++v) {
				for (int u{}; u < static_cast<int>(segmentU); ++u) {
					vertexPositions.push_back({ 
						startOffset 
						+ (vertexOffsetWidth * static_cast<float>(u)) 
						+ (vertexOffsetHeight * static_cast<float>(v))
						});
					vertexNormals.push_back(vertexNormal);
					



					if (v < static_cast<int>(segmentV) - 1 && u < static_cast<int>(segmentU) - 1) {
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
							indices.push_back({ i0, i2, i1 });
							indices.push_back({ i1, i2, i3 });
						}
						else {
							indices.push_back({ i0, i1, i2 });
							indices.push_back({ i1, i3, i2 });
						}
					}
				}
			}
		}
	}
 
	// - new system ----
	Submesh& submesh = m_submeshList[0];
	submesh.ClearSubmeshInformation();
	submesh.SetVertexCount(vertexPositions.size());
	submesh.SetData<glm::vec3>("position", vertexPositions.data(), vertexPositions.size());
	submesh.SetData<glm::vec3>("normal", vertexNormals.data(), vertexPositions.size());
	submesh.SetVertexIndices(indices.data(), indices.size());
}
