#include <arch/core.h>
#include <arch/resources/res_materialManager.h>

void MaterialManager::Init() {

}

GLuint MaterialManager::ResolveMaterial(std::string _materialID) const {

	return 0;
	
}

std::shared_ptr<Material> MaterialManager::CreateGenericMaterial() {
	std::shared_ptr<Material> matPtr { std::make_shared<Material>(Material()) };
	Material& mat{ *matPtr };
	return matPtr;
}

std::shared_ptr<Material> MaterialManager::CreateUnlitMaterial() {
	return std::shared_ptr<Material>();
}

std::shared_ptr<LambertMaterial> MaterialManager::CreateLambertMaterial() {
	std::shared_ptr<LambertMaterial> mat { std::make_shared<LambertMaterial>(LambertMaterial()) };
	ResourceIdentifier resMd = Core::GetInstance().GetResourceManager().AddInternalResource(mat);
	std::shared_ptr<LambertMaterial> copy = mat;
	m_materialStorage.Add(std::move(mat), resMd.m_resourceID);
	return copy;
}

std::shared_ptr<PhongMaterial> MaterialManager::CreatePhongMaterial() {
	std::shared_ptr<PhongMaterial> mat{ std::make_shared<PhongMaterial>(PhongMaterial()) };
	ResourceIdentifier resMd = Core::GetInstance().GetResourceManager().AddInternalResource(mat);
	std::shared_ptr<PhongMaterial> copy = mat;
	m_materialStorage.Add(std::move(mat), resMd.m_resourceID);
	return copy;
}

std::shared_ptr<BlinnPhongMaterial> MaterialManager::CreateBlinnMaterial() {
	std::shared_ptr<BlinnPhongMaterial> mat{ std::make_shared<BlinnPhongMaterial>(BlinnPhongMaterial()) };
	ResourceIdentifier resMd = Core::GetInstance().GetResourceManager().AddInternalResource(mat);
	std::shared_ptr<BlinnPhongMaterial> copy = mat;
	m_materialStorage.Add(std::move(mat), resMd.m_resourceID);
	return copy;
}

std::shared_ptr<Material> MaterialManager::CreateGGXMaterial() {
	return std::shared_ptr<Material>();
}

