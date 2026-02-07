#include <arch/resources/res_mesh_presets/res_mesh_sphere.h>

Sphere::Sphere(float _radius, glm::ivec2 _subdivisions) 
    : m_radius{ _radius }, m_subdivisions{_subdivisions} {
    m_name = "Sphere";
    Init();
}

void Sphere::Init() {


	UpdateVertexData();
}



void Sphere::UpdateVertexData() {
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

    SetData<float>("position", reinterpret_cast<float*>(vertexPositions.data()), vertexPositions.size() * 3);
    SetData<float>("normal", reinterpret_cast<float*>(vertexNormals.data()), vertexPositions.size() * 3);
}
