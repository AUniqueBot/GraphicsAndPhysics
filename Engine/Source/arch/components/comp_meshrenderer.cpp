#include <arch/components/comp_meshrenderer.h>
#include <util/util_serialisation.h>
#include <arch/resources/res_mesh_presets/res_mesh_cube.h>
#include <arch/resources/res_material_presets/res_material_lambert.h>


// - method function ------------------------

MeshRenderer::MeshRenderer() {
	SetComponentID(ComponentType::RENDER);
}

void MeshRenderer::Init() {
	LOG_INFO("Running Init");
	//m_mesh.Init();
	m_mesh = std::make_shared<Mesh>(Mesh{});
	m_mesh->Init();
}

void MeshRenderer::End() {
	
}


void MeshRenderer::Render(
	const glm::vec4& _objectMatrix, 
	const glm::vec4& _projectionMatrix, 
	const glm::vec4& _cameraMatrix
) {
	
}

Material& MeshRenderer::GetDefaultMaterial() {

	static LambertMaterial m_defaultMaterial{};
	static bool defaultMatInit	{ false };
	if (!defaultMatInit) {
		m_defaultMaterial.Init();

		// - init is true -----------------------------------------
		defaultMatInit = true;
	}
	return m_defaultMaterial;
}





