#include <arch/resources/res_materialManager.h>
#include <arch/resources/res_shaderManager.h>
#include <arch/resources/res_texture/res_texture2d.h>
#include <rapidjson/istreamwrapper.h>


void MaterialManager::Init() {
	RegisterFileExtension(".material");
}

GLuint MaterialManager::ResolveMaterial(std::string _materialID) const {

	return 0;
	
}

MaterialHandle MaterialManager::CreateGenericMaterial() {
	std::shared_ptr<Material> matPtr { std::make_shared<Material>() };
	MaterialHandle mat(RegisterResource(matPtr));
	return mat;
}

MaterialHandle MaterialManager::CreateUnlitMaterial() {
	return MaterialHandle(std::nullopt);
}

LambertMaterialHandle MaterialManager::CreateLambertMaterial() {
	std::shared_ptr<LambertMaterial> mat { std::make_shared<LambertMaterial>() };
	LambertMaterialHandle res (RegisterResource(mat));
	return res;
}

PhongMaterialHandle MaterialManager::CreatePhongMaterial() {
	std::shared_ptr<PhongMaterial> mat{ std::make_shared<PhongMaterial>() };
	PhongMaterialHandle res(RegisterResource(mat));
	return res;
}

BlinnPhongMaterialHandle MaterialManager::CreateBlinnMaterial() {
	std::shared_ptr<BlinnPhongMaterial> mat{ std::make_shared<BlinnPhongMaterial>() };
	BlinnPhongMaterialHandle handle(m_resourceManager.AddInternalResource(mat));
	return handle;
}

MaterialHandle MaterialManager::CreateGGXMaterial() {
	return MaterialHandle(std::nullopt);
}

MaterialHandle MaterialManager::LoadMaterial(
	const rapidjson::Value& _materialData,
	RES_ID _existingId
) {
	const rapidjson::Value& shader = _materialData["shader_id"];
	std::shared_ptr<Material> mat;
	if (shader.IsString()) {
		// shader uses an alias (human read), load that.
		std::string alias = shader.GetString();
		const rapidjson::Value& props = _materialData["properties"];
		if (alias == ShaderConstants::C_ID_LAMBERTSHADERPROG) {
			mat = LoadLambertMaterial(props);
		}
		else if (alias == ShaderConstants::C_ID_PHONGSHADERPROG) {
			mat = LoadPhongMaterial(props);
		}
		else if (alias == ShaderConstants::C_ID_BLINNPHONGSHADERPROG) {
			mat = LoadBlinnMaterial(props);
		}

	}

	else {
		// get resource id.
		RES_ID id = shader.GetInt();
		std::shared_ptr<ShaderProgram> shaderProg = 
			dynamic_pointer_cast<ShaderProgram>(m_resourceManager.GetResource(id));
		if (shaderProg) {
			mat->SetShaderProgram(shaderProg->GetShaderProgramID());
		}
		// custom material...

	}

	if (!mat) {
		return MaterialHandle();
	}

	if (_existingId != ResourceConstants::C_RES_INVALID_ID) {
		mat->ResourceID(_existingId);
	}


	mat->Name(_materialData["name"].GetString());
	MaterialHandle handle(m_resourceManager.AddInternalResource(mat));
	return handle;
}


std::shared_ptr<LambertMaterial> MaterialManager::LoadLambertMaterial(const rapidjson::Value& _materialData) {
	using namespace MaterialLookupConstants;
	auto mat = std::make_shared<LambertMaterial>();

	if (_materialData.HasMember(C_MAT_U_ALBEDO_COL)) {
		const auto& col = _materialData[C_MAT_U_ALBEDO_COL];
		
		if (col.IsArray()) {
			glm::vec4 color{};			
			color.r = col[0].GetFloat();
			color.g = col[1].GetFloat();
			color.b = col[2].GetFloat();
			color.a = col[3].GetFloat();
			mat->Color(color);
		}
		else {
			std::string hex = col.GetString();
			unsigned long color = std::stoul(hex, nullptr, 16);
			mat->Color(color);
		}
	}
	if (_materialData.HasMember(C_MAT_U_ALBEDO_TEX)) {
		// get a texture handle.
		auto& tex = _materialData[C_MAT_U_ALBEDO_TEX];
		if (!tex.IsNull()) {
			RES_ID id = tex.GetUint64();
			Texture2DHandle handle(m_resourceManager.GetResourceIdentifier(id));
			// assign texture.
			// mat->SetAlbedoTexture(handle);
			// mat->SetUsesColor(false);
		}
	}


	return mat;
}

std::shared_ptr<PhongMaterial> MaterialManager::LoadPhongMaterial(const rapidjson::Value& _materialData) {
	using namespace MaterialLookupConstants;
	auto mat = std::make_shared<PhongMaterial>();

	if (_materialData.HasMember(C_MAT_U_ALBEDO_COL)) {
		const auto& col = _materialData[C_MAT_U_ALBEDO_COL];

		if (col.IsArray()) {
			glm::vec4 color{};
			color.r = col[0].GetFloat();
			color.g = col[1].GetFloat();
			color.b = col[2].GetFloat();
			color.a = col[3].GetFloat();
			mat->Color(color);
		}
		else if (col.IsString()) {
			std::string hex = col.GetString();
			unsigned long col = std::stoul(hex, nullptr, 0);
			mat->Color(col);
		}
	}
	if (_materialData.HasMember(C_MAT_U_ALBEDO_TEX)) {
		// get a texture handle.
		auto& tex = _materialData[C_MAT_U_ALBEDO_TEX];
		if (!tex.IsNull()) {
			RES_ID id = tex.GetUint64();
			Texture2DHandle handle(m_resourceManager.GetResourceIdentifier(id));
			// assign texture.
			// mat->SetAlbedoTexture(handle);
			// mat->SetUsesColor(false);
		}
	}



	if (_materialData.HasMember(C_MAT_U_SPECULARCOL)) {
		const auto& col = _materialData[C_MAT_U_SPECULARCOL];
		if (col.IsArray()) {
			glm::vec4 color{};
			color.r = col[0].GetFloat();
			color.g = col[1].GetFloat();
			color.b = col[2].GetFloat();
			color.a = col[3].GetFloat();
			mat->Specular(color);
		}
		else if (col.IsString()) {
			std::string hex = col.GetString();
			unsigned long col = std::stoul(hex, nullptr, 0);
			mat->Specular(col);
		}
	}
	if (_materialData.HasMember(C_MAT_U_SPECULARTEX)) {
		// get a texture handle.
		auto& tex = _materialData[C_MAT_U_SPECULARTEX];
		if (!tex.IsNull()) {
			RES_ID id = tex.GetUint64();
			Texture2DHandle handle(m_resourceManager.GetResourceIdentifier(id));
			// assign texture.
			// mat->SetAlbedoTexture(handle);
			// mat->SetUsesColor(false);
		}
	}


	if (_materialData.HasMember(C_MAT_U_EXPONENT)) {
		mat->Exponent(_materialData[C_MAT_U_EXPONENT].GetInt());
	}



	return mat;
}

std::shared_ptr<BlinnPhongMaterial> MaterialManager::LoadBlinnMaterial(const rapidjson::Value& _materialData) {
	using namespace MaterialLookupConstants;
	auto mat = std::make_shared<BlinnPhongMaterial>();

	if (_materialData.HasMember(C_MAT_U_ALBEDO_COL)) {
		const auto& col = _materialData[C_MAT_U_ALBEDO_COL];

		if (col.IsArray()) {
			glm::vec4 color{};
			color.r = col[0].GetFloat();
			color.g = col[1].GetFloat();
			color.b = col[2].GetFloat();
			color.a = col[3].GetFloat();
			mat->Color(color);
		}
		else if (col.IsString()) {
			std::string hex = col.GetString();
			unsigned long col = std::stoul(hex);
			mat->Color(col);
		}
	}
	if (_materialData.HasMember(C_MAT_U_ALBEDO_TEX)) {
		// get a texture handle.
		auto& tex = _materialData[C_MAT_U_ALBEDO_TEX];
		if (!tex.IsNull()) {
			RES_ID id = tex.GetUint64();
			Texture2DHandle handle(m_resourceManager.GetResourceIdentifier(id));
			// assign texture.
			// mat->SetAlbedoTexture(handle);
			// mat->SetUsesColor(false);
		}
	}



	if (_materialData.HasMember(C_MAT_U_SPECULARCOL)) {
		const auto& col = _materialData[C_MAT_U_SPECULARCOL];
		if (col.IsArray()) {
			glm::vec4 color{};
			color.r = col[0].GetFloat();
			color.g = col[1].GetFloat();
			color.b = col[2].GetFloat();
			color.a = col[3].GetFloat();
			mat->Specular(color);
		}
		else if (col.IsString()) {
			std::string hex = col.GetString();
			unsigned long col = std::stoul(hex);
			mat->Specular(col);
		}
	}
	if (_materialData.HasMember(C_MAT_U_SPECULARTEX)) {
		// get a texture handle.
		auto& tex = _materialData[C_MAT_U_SPECULARTEX];
		if (!tex.IsNull()) {
			RES_ID id = tex.GetUint64();
			Texture2DHandle handle(m_resourceManager.GetResourceIdentifier(id));
			// assign texture.
			// mat->SetAlbedoTexture(handle);
			// mat->SetUsesColor(false);
		}
	}


	if (_materialData.HasMember(C_MAT_U_EXPONENT)) {
		mat->Exponent(_materialData[C_MAT_U_EXPONENT].GetInt());
	}

	return mat;
}

std::shared_ptr<Material> MaterialManager::LoadGGXMaterial(const rapidjson::Value& _materialData) {
	return std::shared_ptr<Material>();
}

void MaterialManager::LoadResource(const MetafileData& _data) {
	namespace fs = std::filesystem;
	fs::path path = _data.path;
	std::ifstream ifs(path);
	if (!ifs.is_open()) {
		ifs.close();
		return;
	}
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document doc;
	doc.ParseStream(isw);
	ifs.close();
	MaterialHandle handle = LoadMaterial(doc, _data.id);
	handle.GetBaseResource()->ResourcePath(path);
}

