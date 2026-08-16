#include <arch/resources/res_mesh_presets/res_mesh_sphere.h>

SphereRes::SphereRes(SphereCreationProps _props)
    : m_radius{ _props.radius }, m_subdivisions{ _props.subdivisions } {
    m_name = "Sphere";
}

void SphereRes::Init() {
    SetSubmeshCount(1);
    AddSubmesh(Submesh());
	UpdateVertexData();
}


const float& SphereRes::GetRadius() const {
    return m_radius;
}
void SphereRes::SetRadius(float _radius) {
    if (m_radius == _radius) return;
    m_radius = _radius;
}

const int& SphereRes::GetSubdivisionsX() const {
    return m_subdivisions.x;
}
const int& SphereRes::GetSubdivisionsY() const {
    return m_subdivisions.y;
}
const glm::ivec2& SphereRes::GetSubdivisions() const {
    return m_subdivisions;
}

void SphereRes::SetSubdivisionsX(int _divs) {
    if (m_subdivisions.x == _divs) return;
    m_subdivisions.x = _divs;
    UpdateVertexData();
}
void SphereRes::SetSubdivisionsY(int _divs) {
    if (m_subdivisions.y == _divs) return;
    m_subdivisions.y = _divs;
    UpdateVertexData();
}
void SphereRes::SetSubdivisions(glm::ivec2 _divs) {
    if (m_subdivisions == _divs) return;
    m_subdivisions = _divs;
    UpdateVertexData();
}


void SphereRes::UpdateVertexData() {
	ClearMeshInformation();

    const unsigned segX = m_subdivisions.x;
    const unsigned segY = m_subdivisions.y;
    const float radius = m_radius;
    std::vector<glm::vec3> vertexPositions;
    std::vector<glm::vec3> vertexNormals;
    // Generate vertices
    for (unsigned y = 0; y <= segY; ++y) {
        float v = float(y) / float(segY);
        float phi = v * glm::pi<float>();   // [0, PI]

        for (unsigned x = 0; x <= segX; ++x) {
            float u = float(x) / float(segX);
            float theta = u * glm::two_pi<float>(); // [0, 2PI]

            glm::vec3 pos;
            pos.x = radius * sin(phi) * cos(theta);
            pos.y = radius * cos(phi);
            pos.z = radius * sin(phi) * sin(theta);

            glm::vec3 normal = glm::normalize(pos);

            vertexPositions.push_back(pos);
            vertexNormals.push_back(normal);
        }
    }

    auto index = [&](unsigned x, unsigned y) {
        return y * (segX + 1) + x;
    };

    for (unsigned y = 0; y < segY; ++y) {
        for (unsigned x = 0; x < segX; ++x) {

            unsigned i0 = index(x, y);
            unsigned i1 = index(x + 1, y);
            unsigned i2 = index(x, y + 1);
            unsigned i3 = index(x + 1, y + 1);

            // Skip degenerate triangles at poles automatically
            if (y != 0) {
                m_indices.emplace_back(i0, i2, i1);
            }

            if (y != segY - 1) {
                m_indices.emplace_back(i1, i2, i3);
            }
        }
    }


    // - new system ----
    Submesh& submesh = m_submeshList[0];
    submesh.ClearSubmeshInformation();
    submesh.SetVertexCount(vertexPositions.size());
    submesh.SetData<glm::vec3>("position", vertexPositions.data(), vertexPositions.size());
    submesh.SetData<glm::vec3>("normal", vertexNormals.data(), vertexPositions.size());
    submesh.SetVertexIndices(m_indices.data(), m_indices.size());
}
