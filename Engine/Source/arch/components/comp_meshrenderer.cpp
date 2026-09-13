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

void MeshRenderer::SetMesh(const MeshHandle& _mesh) {
	m_mesh = _mesh;
}

MeshHandle& MeshRenderer::GetMesh() {
	return m_mesh; 
}

const MeshHandle& MeshRenderer::GetMesh() const {
	return m_mesh; 
}


std::vector<MaterialHandle>& MeshRenderer::GetMaterialList() { 
	return m_materials; 
}
const std::vector<MaterialHandle>& MeshRenderer::GetMaterialList() const { 
	return m_materials; 
}

void MeshRenderer::AddMaterial(MaterialHandle _material) {
	m_materials.push_back(_material);
}

void MeshRenderer::RemoveMaterial(MaterialHandle _material) {
	const auto& itr{ std::find(m_materials.begin(), m_materials.end(), _material) };
	if (itr == m_materials.end()) return;
	std::rotate(itr, itr + 1, m_materials.end());
	m_materials.pop_back();
}

void MeshRenderer::RemoveMaterial(int _index) {
	if (_index < m_materials.size()) {
		return;
	}
	auto itr = m_materials.begin() + _index;
	std::rotate(itr, itr + 1, m_materials.end());
	m_materials.pop_back();

}

MaterialHandle MeshRenderer::GetMaterial(int _index) {
	if (m_materials.size() <= _index) return MaterialHandle();
	return m_materials[_index];
}

const MaterialHandle MeshRenderer::GetMaterial(int _index) const {
	if (m_materials.size() <= _index) return MaterialHandle();
	return m_materials.at(_index);
}



MaterialRes& MeshRenderer::GetDefaultMaterial() {
	static LambertMaterialRes m_defaultMaterial{};
	static bool defaultMatInit	{ false };
	if (!defaultMatInit) {
		m_defaultMaterial.Color(0xaaaaaaff);
		// - init is true -----------------------------------------

		defaultMatInit = true;
	}
	return m_defaultMaterial;
}

// ------------------------------------------------------------------------------------------------------------


const bool& MeshRenderer::CastShadows() const { 
	return m_castShadows; 
};
void MeshRenderer::CastShadows(const bool& _cast) { 
	m_castShadows = _cast; 
};

const bool& MeshRenderer::ReceiveShadows() const {
	return m_receiveShadows;
}
void MeshRenderer::ReceiveShadows(const bool& _setting) {
	m_receiveShadows = _setting;
}

const bool& MeshRenderer::StaticShadows() const {
	return m_staticShadows;
}

void MeshRenderer::StaticShadows(const bool& _setting) {
	m_staticShadows = _setting;
}


// ------------------------------------------------------------------------------------------------------------

std::vector<PropertyMD::Property>& MeshRenderer::GetProps() {
	using namespace PropertyMD;
	using MaterialList = std::vector<MaterialHandle>;

	static std::vector<PropertyMD::Property> props{
		MakeResourceProperty<MeshRenderer>(
			"Mesh",
			MeshRes::GetResourceTypeID(),
			static_cast<const MeshHandle & (MeshRenderer::*)()const>(&MeshRenderer::GetMesh),
			&MeshRenderer::SetMesh
		),
		MakeListProperty<MeshRenderer, MaterialHandle>(
			"Materials",
			PropertyType::ResourceHandle,
			static_cast<MaterialList&(MeshRenderer::*)()>(&MeshRenderer::GetMaterialList),
			static_cast<void(MeshRenderer::*)(MaterialHandle)>(&MeshRenderer::AddMaterial),
			static_cast<void(MeshRenderer::*)(int)>(&MeshRenderer::RemoveMaterial)
		),
		MakeProperty<MeshRenderer>(
			"Cast Shadow",
			PropertyType::Boolean,
			Shape::Scalar, 1,
			static_cast<const bool& (MeshRenderer::*)() const>(&MeshRenderer::CastShadows),
			static_cast<void (MeshRenderer::*)(const bool&)>(&MeshRenderer::CastShadows)
		)
	};
		
	// list out the props for the materials.
	return props;
}

