#include <arch/components/comp_meshrenderer.h>
#include <util/util_serialisation.h>
#include <arch/resources/res_mesh_presets/res_mesh_cube.h>
#include <arch/resources/res_material_presets/res_material_lambert.h>
#include <arch/resources/res_material_presets/res_material_phong.h>


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
	m_materials.push_back(_material);
}

void MeshRenderer::RemoveMaterial(std::shared_ptr<Material> _material) {
	const auto& itr{ std::find(m_materials.begin(), m_materials.end(), _material) };
	if (itr == m_materials.end()) return;
	std::rotate(itr, itr + 1, m_materials.end());
	m_materials.pop_back();
}



Material& MeshRenderer::GetDefaultMaterial() {
	static LambertMaterial m_defaultMaterial{};
	static bool defaultMatInit	{ false };
	if (!defaultMatInit) {
		m_defaultMaterial.Color(0xaaaaaaff);
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
			static_cast<std::vector<std::shared_ptr<Material>>&(MeshRenderer::*)()>(&MeshRenderer::GetMaterialList),
			PropertyType::Object 
		),
	};
	
	// list out the props for the materials.
	return props;
}

