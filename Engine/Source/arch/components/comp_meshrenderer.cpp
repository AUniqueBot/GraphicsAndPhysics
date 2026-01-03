#include <arch/components/comp_meshrenderer.h>
#include <util/util_serialisation.h>
#include <arch/resources/res_mesh_presets/res_mesh_cube.h>
#include <arch/resources/res_material_presets/res_material_lambert.h>


// - method function ------------------------

MeshRenderer::MeshRenderer() {
}

void MeshRenderer::Init() {
	LOG_INFO("Running Init");
	//m_mesh.Init();
	//m_mesh = std::make_shared<Mesh>(Mesh{});
	//m_mesh->Init();
}

void MeshRenderer::End() {
	
}

void MeshRenderer::SetMesh(std::shared_ptr<Mesh> _mesh) {
	m_mesh = _mesh;

}

std::shared_ptr<const Mesh> MeshRenderer::GetMesh() const { 
	return m_mesh; 
}
std::shared_ptr<Mesh> MeshRenderer::GetMesh() { 
	return m_mesh; 
}

void MeshRenderer::Render(
	const glm::mat4& _objectMatrix, 
	const glm::mat4& _projectionMatrix, 
	const glm::mat4& _cameraMatrix
) {

	// use the default material and render.
	if (m_materials.size() == 0) {
		const Material& mat = GetDefaultMaterial();
		mat.Render(
			_objectMatrix,
			_projectionMatrix,
			_cameraMatrix
		);
 
		glDrawElements(GL_TRIANGLES, m_mesh->GetIndexDataCount(), GL_UNSIGNED_INT, 0);
		return;
	}
	// go through all materials
	for (const std::shared_ptr<Material> matPtr: m_materials) {
		matPtr->Render(
			_objectMatrix,
			_projectionMatrix,
			_cameraMatrix
		);


		// TODO - render based on materials indices.


		glDrawElements(GL_TRIANGLES, m_mesh->GetIndexDataCount(), GL_UNSIGNED_INT, 0);

	}
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





