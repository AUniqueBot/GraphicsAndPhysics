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


void MeshRenderer::AddMaterial(std::shared_ptr<Material> _material) {
	_material->Init();
	m_materials.push_back(_material);

}

void MeshRenderer::RemoveMaterial(std::shared_ptr<Material> _material) {
	const auto& itr{ std::find(m_materials.begin(), m_materials.end(), _material) };
	if (itr == m_materials.end()) return;
	std::rotate(itr, itr + 1, m_materials.end());
	m_materials.pop_back();
}

void MeshRenderer::ApplyShadowMap(const ShadowMap& _shadowMap) const {
	if (m_materials.size() == 0) {
 		const Material& mat = GetDefaultMaterial();
		mat.ApplyShadowMap(_shadowMap);
		return;
	}

	for (const std::shared_ptr<Material>& matPtr : m_materials) {
		matPtr->ApplyShadowMap(_shadowMap);
	}
}

void MeshRenderer::Render() {
	GLsizei meshFloatCount { static_cast<GLsizei>(m_mesh->GetIndexDataCount() * 3) };
	// use the default material and render.
	if (m_materials.size() == 0) {
		const Material& mat = GetDefaultMaterial();
		mat.ApplyUniforms();
		glDrawElements(GL_TRIANGLES, meshFloatCount, GL_UNSIGNED_INT, 0);
		return;
	}
	// go through all materials
	for (const std::shared_ptr<Material> matPtr: m_materials) {
		matPtr->ApplyUniforms();
		glDrawElements(GL_TRIANGLES, (m_mesh->GetIndexDataCount() * 3), GL_UNSIGNED_INT, 0);
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




std::vector<PropertyMD::Property>& MeshRenderer::GetProps() {
	using namespace PropertyMD;
	static std::vector<PropertyMD::Property> props{
		MakeListProperty<MeshRenderer, std::shared_ptr<Material>>(
			"Materials",
			static_cast<std::vector<std::shared_ptr<Material>>&(MeshRenderer::*)()>(&MeshRenderer::GetMaterialList)
		),
	};
	
	return props;
}

