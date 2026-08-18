
#include <UI_Core.h>
#include <Widgets/UIWidget_Inspector.h>
#include <imgui_stdlib.h>


#include <arch/components/comp_transform.h>
#include <arch/components/comp_meshrenderer.h>
#include <arch/components/comp_light.h>
#include <arch/components/comp_camera.h>
#include <arch/resources/res_mesh_presets/res_mesh_types.h>
#include <arch/resources/res_material_presets/res_material_lambert.h>
#include <arch/resources/res_material_presets/res_material_phong.h>


namespace Raw {

	static bool DrawPropertyInt(const std::string& _label, void* _value, int _compCount, bool _draggable) {
		switch (_compCount) {
		case 1:
			return _draggable ?
				ImGui::DragInt(_label.c_str(), static_cast<int*>(_value)) :
				ImGui::InputInt(_label.c_str(), static_cast<int*>(_value));

		case 2:
			return _draggable ?
				ImGui::DragInt2(_label.c_str(), static_cast<int*>(_value)) :
				ImGui::InputInt2(_label.c_str(), static_cast<int*>(_value));

		case 3:
			return _draggable ?
				ImGui::DragInt3(_label.c_str(), static_cast<int*>(_value)) :
				ImGui::InputInt3(_label.c_str(), static_cast<int*>(_value));

		case 4:
			return _draggable ?
				ImGui::DragInt4(_label.c_str(), static_cast<int*>(_value)) :
				ImGui::InputInt4(_label.c_str(), static_cast<int*>(_value));
		}

		return false;
	}


	static bool DrawPropertyFloat(const std::string& _label, void* _value, int _compCount, bool _draggable) {
		switch (_compCount) {
		case 1:
			return _draggable ?
				ImGui::DragFloat(_label.c_str(), static_cast<float*>(_value)) :
				ImGui::InputFloat(_label.c_str(), static_cast<float*>(_value));

		case 2:
			return _draggable ?
				ImGui::DragFloat2(_label.c_str(), static_cast<float*>(_value)) :
				ImGui::InputFloat2(_label.c_str(), static_cast<float*>(_value));

		case 3:
			return _draggable ?
				ImGui::DragFloat3(_label.c_str(), static_cast<float*>(_value)) :
				ImGui::InputFloat3(_label.c_str(), static_cast<float*>(_value));

		case 4:
			return _draggable ?
				ImGui::DragFloat4(_label.c_str(), static_cast<float*>(_value)) :
				ImGui::InputFloat4(_label.c_str(), static_cast<float*>(_value));
		}

		return false;
	}


	static bool DrawPropertyDouble(const std::string& _label, void* _value, int, bool) {
		return ImGui::InputDouble(_label.c_str(), static_cast<double*>(_value));
	}


	static bool DrawPropertyColor(const std::string& _label, void* _value, int _compCount, bool) {
		switch (_compCount) {
		case 3:
			return ImGui::ColorEdit3(_label.c_str(), static_cast<float*>(_value));

		case 4:
			return ImGui::ColorEdit4(_label.c_str(), static_cast<float*>(_value));
		}

		return false;
	}


	static bool DrawPropertyBoolean(const std::string& _label, void* _value, int, bool) {
		return ImGui::Checkbox(_label.c_str(), static_cast<bool*>(_value));
	}


	static bool DrawPropertyString(const std::string& _label, void* _value, int, bool) {
		return ImGui::InputText(_label.c_str(), static_cast<std::string*>(_value));
	}

}

UIWidget_Inspector::UIWidget_Inspector(std::string _widgetName) : UIWidget(_widgetName) {

}



void UIWidget_Inspector::Init() {

}


void UIWidget_Inspector::Draw() {
	using namespace ImGui;
	UI_Core* puic = UICore();
	Core* papc = ApplicationCore();
	
	if (!puic || !papc) return;

	std::function<bool()> EnterOrTabPressed = []() { 
		return 
			ImGui::IsKeyPressed(ImGuiKey_Enter) || 
			ImGui::IsKeyPressed(ImGuiKey_Tab) ||
			ImGui::IsKeyPressed(ImGuiKey_KeypadEnter); 
	};



	UI_Core& uic = *puic;
	Core& core = *papc;
	EntityRegistry& er = core.GetRegistry();
	EntityID selectedID = core.GetRegistry().SelectedEntity();
	EntityView selectedObj = core.GetRegistry().GetEntity(selectedID);




	if (!selectedObj) {
		Text("No object selected with ID [%i]", selectedID); 
		return;
	}
	



	Entity& obj = *selectedObj;
	std::string s{ selectedObj->Name() };
	if (InputText("Object Name", &s) && EnterOrTabPressed()) {
		obj.Name(s);
	}

	Text("ID: [%i]", obj.GetID());

	// render components here.


	for (auto& compHandle : er.GetEntityComponents(selectedID)) {
		Component* comp{ compHandle.m_componentPtr };
		ImGui::Separator();

		const std::vector<PropertyMD::Property>& props{ comp->GetProperties() };
		if (!props.size()) {
			continue;
		}
		if (ImGui::CollapsingHeader(compHandle.m_componentMetadata->GetComponentName().c_str())) {
			for (const PropertyMD::Property& prop : props) {
				ImGui::SeparatorText(prop.m_name.c_str());
				DrawPropertyElement(comp, prop, prop.m_name);
			}
		}
		
	}




	auto meshV = obj.GetComponent<MeshRenderer>();
	if (meshV) {
		MeshRenderer& mr = *meshV;
		ResourceManager& resmgr = papc->GetResourceManager();
		MeshManager& meshmgr = papc->GetAssetManager().GetMeshManager();
		MaterialManager& matMgr = papc->GetAssetManager().GetMaterialManager();
		// mesh handling.
		RES_ID meshId = mr.GetMesh();
		std::shared_ptr<MeshRes> selectedMesh = static_pointer_cast<MeshRes>(resmgr.GetResource(meshId));


		std::string selectedMeshName = selectedMesh->Name();
		RES_ID selectedMeshID = selectedMesh->ResourceID();
		if (BeginCombo("Mesh##Inspector_Meshes", selectedMeshName.c_str())) {
			const auto& resPool = resmgr.GetResourcePool();
			const auto& meshIDs = resmgr.GetResourcePoolManifest(MeshRes::GetResourceTypeID());
			
			// first are the default meshes
			bool isSelected = false;
			for (unsigned i{}; i < Primitive::__COUNT; ++i) {

				std::shared_ptr<MeshRes> mesh	{};
				std::string primitiveType	{};
				switch (i) {
					case Primitive::CUBE:
						mesh.reset(new CubeRes());
						primitiveType = "Cube";
						break;
					case Primitive::SPHERE:
						mesh.reset(new SphereRes());
						primitiveType = "Sphere";
						break;
					case Primitive::PLANE:
						primitiveType = "Plane";
						mesh.reset(new PlaneRes());
						break;
					case Primitive::ICOSPHERE:
						continue;
						primitiveType = "WIP - Icosphere";
						//mesh.reset(new Plane());
						break;
				}
				
				isSelected = selectedMeshName == primitiveType;
				if (Selectable(primitiveType.c_str())) {
					 mr.SetMesh(mesh->ResourceID());
				}
			}
			

			for (const RES_ID& id : meshIDs) {
				std::string imguiMeshID	{ "##meshID" };
				imguiMeshID += std::to_string(id);
				PushID(imguiMeshID.c_str());

				const auto& mesh = std::dynamic_pointer_cast<MeshRes>(resPool.at(id));
				std::string name { mesh->ResourcePath().filename().string()  };

				isSelected = selectedMeshID == id;
				if (Selectable(name.c_str(), isSelected)) {
					mesh->Init();
					mr.SetMesh(mesh->ResourceID());
				}
				PopID();
			}
			EndCombo();
		}
	}
}


void UIWidget_Inspector::Exit() {

}



void UIWidget_Inspector::DrawPropertyElement(void* object, const PropertyMD::Property& prop, const std::string& key) {
	using namespace PropertyMD;
	std::string name{prop.m_name};
	name += "##";
	name += key;
	
	
	if (prop.m_list.m_valid) {
		DrawPropertiesDynamicList(object, prop, name);
		return;
	}


	if (prop.m_isEnum) {
		DrawPropertyOptions(object, prop, name);
		return;
	}
	
	switch (prop.m_type) {
	case PropertyType::Color:
		DrawPropertyColor(object, prop, name);
		break;
	case PropertyType::Int:
		DrawPropertyInt(object, prop, name);
		break;
	case PropertyType::Float:
		DrawPropertyFloat(object, prop, name);
		break;
	case PropertyType::Double:
		DrawPropertyDouble(object, prop, name);
		break;
	case PropertyType::Boolean:
		DrawPropertyBoolean(object, prop, name);
		break;
	case PropertyType::String:
		DrawPropertyString(object, prop, name);
		break;
	case PropertyType::Object:
		DrawPropertyObject(object, prop, name);
		break;
	case PropertyType::ResourceHandle:
		DrawPropertyResourceHandle(object, prop, name);
		break;
	default:
		break;
	}

}


void UIWidget_Inspector::DrawPropertyInt(void* object, const PropertyMD::Property& prop, const std::string& key) {
	switch (prop.m_componentCount) {

	case 1: {
		int val{};
		prop.m_get(object, &val);
		if (Raw::DrawPropertyInt(key, &val, 1, prop.m_draggable))
			prop.m_set(object, &val);
		break;
	}

	case 2: {
		glm::ivec2 val{};
		prop.m_get(object, &val);
		if (Raw::DrawPropertyInt(key, glm::value_ptr(val), 2, prop.m_draggable))
			prop.m_set(object, &val);
		break;
	}

	case 3: {
		glm::ivec3 val{};
		prop.m_get(object, &val);
		if (Raw::DrawPropertyInt(key, glm::value_ptr(val), 3, prop.m_draggable))
			prop.m_set(object, &val);
		break;
	}

	case 4: {
		glm::ivec4 val{};
		prop.m_get(object, &val);
		if (Raw::DrawPropertyInt(key, glm::value_ptr(val), 4, prop.m_draggable))
			prop.m_set(object, &val);

		break;
	}
	}
}


void UIWidget_Inspector::DrawPropertyFloat(void* object, const PropertyMD::Property& prop, const std::string& key) {
	switch (prop.m_componentCount) {

	case 1: {
		float val{};
		prop.m_get(object, &val);
		if (Raw::DrawPropertyFloat(key, &val, 1, prop.m_draggable))
			prop.m_set(object, &val);
		break;
	}

	case 2: {
		glm::vec2 val{};
		prop.m_get(object, &val);
		if (Raw::DrawPropertyFloat(key, glm::value_ptr(val), 2, prop.m_draggable))
			prop.m_set(object, &val);
		break;
	}

	case 3: {
		glm::vec3 val{};
		prop.m_get(object, &val);
		if (Raw::DrawPropertyFloat(key, glm::value_ptr(val), 3, prop.m_draggable))
			prop.m_set(object, &val);
		break;
	}

	case 4: {
		glm::vec4 val{};
		prop.m_get(object, &val);
		if (Raw::DrawPropertyFloat(key, glm::value_ptr(val), 4, prop.m_draggable))
			prop.m_set(object, &val);
		break;
	}
	}
}


void UIWidget_Inspector::DrawPropertyDouble(void* object, const PropertyMD::Property& prop, const std::string& key) {
	double val{};
	prop.m_get(object, &val);

	if (Raw::DrawPropertyDouble(key, &val, 1, false))
		prop.m_set(object, &val);
}


void UIWidget_Inspector::DrawPropertyColor(void* object, const PropertyMD::Property& prop, const std::string& key) {
	if (prop.m_componentCount == 3) {
		glm::vec3 val{};
		prop.m_get(object, &val);

		if (Raw::DrawPropertyColor(key, glm::value_ptr(val), 3, false))
			prop.m_set(object, &val);
	}
	else {
		glm::vec4 val{};
		prop.m_get(object, &val);

		if (Raw::DrawPropertyColor(key, glm::value_ptr(val), 4, false))
			prop.m_set(object, &val);
	}
}


void UIWidget_Inspector::DrawPropertyBoolean(void* object, const PropertyMD::Property& prop, const std::string& key) {
	bool val{};
	prop.m_get(object, &val);
	if (Raw::DrawPropertyBoolean(key, &val, 1, false))
		prop.m_set(object, &val);
}


void UIWidget_Inspector::DrawPropertyString(void* object, const PropertyMD::Property& prop, const std::string& key) {
	std::string val{};
	prop.m_get(object, &val);

	if (Raw::DrawPropertyString(key, &val, 1, false))
		prop.m_set(object, &val);
}


void UIWidget_Inspector::DrawPropertyOptions(void* object, const PropertyMD::Property& prop, const std::string& key) {
	int val{};
	prop.m_get(object, &val);
	const char* currentOption{};

	// search id.
	for (const PropertyMD::Option& option : prop.m_options) {
		if (val == option.value) {
			currentOption = option.label;
		}
	}
	if (ImGui::BeginCombo(key.c_str(), currentOption)) {
		for (const PropertyMD::Option& option: prop.m_options) {
			if (ImGui::Selectable(option.label, val == option.value)) {
				prop.m_set(object, &option.value);
			}
		}

		ImGui::EndCombo();
	}
}

void UIWidget_Inspector::DrawPropertyObject(void* object, const PropertyMD::Property& prop, const std::string&) {
	Inspectable* data = reinterpret_cast<Inspectable*>(object);
	for (auto& prop : data->GetProperties()) {
		DrawPropertyElement(object, prop, prop.m_name);
	}
}

void UIWidget_Inspector::DrawPropertyResourceHandle(
	void* object, 
	const PropertyMD::Property& prop, 
	const std::string& key) {
	ResourceHandle* handle = reinterpret_cast<ResourceHandle*>(object);
	if (!handle) return;
	std::shared_ptr<BaseResource> res = handle->GetBaseResource();
	if (!res) return;
	for (auto& prop : res->GetProperties()) {
		DrawPropertyElement((void*)res.get(), prop, prop.m_name);
	}

}

void UIWidget_Inspector::DrawPropertiesDynamicList(void* object, const PropertyMD::Property& prop, const std::string& key) {
	// explain how it works.
	if (!prop.m_list.m_valid) return;

	void* val{};
	const PropertyMD::Property::List& ls{ prop.m_list };
	val = ls.m_listAccessor(object);
	ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
	std::string name = prop.m_name + "#Table";
	
	
	if (ImGui::BeginTable(name.c_str(), 2, flags)) {
		auto& list = prop.m_list;
		size_t size = static_cast<size_t>(list.m_size(object));
		
		ImGui::TableSetupColumn("##Index", ImGuiTableColumnFlags_WidthFixed, 40.0f);
		ImGui::TableSetupColumn("Element", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableHeadersRow();

		for (size_t i{}; i < size; ++i) {
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%i", size);
			ImGui::TableSetColumnIndex(1);

			void* currentElement = list.m_get(object, static_cast<int>(i));
			std::string elementName = "##" + prop.m_name + std::to_string(i);
			// draw your element here.
			switch (list.m_type) {
			case PropertyMD::PropertyType::Boolean:
				Raw::DrawPropertyBoolean(elementName, currentElement, prop.m_componentCount, prop.m_draggable);
				break;
			case PropertyMD::PropertyType::Color:
				Raw::DrawPropertyColor(elementName, currentElement, prop.m_componentCount, prop.m_draggable);
				break;
			case PropertyMD::PropertyType::Double:
				Raw::DrawPropertyDouble(elementName, currentElement, prop.m_componentCount, prop.m_draggable);
				break;
			case PropertyMD::PropertyType::Float:
				Raw::DrawPropertyFloat(elementName, currentElement, prop.m_componentCount, prop.m_draggable);
				break;
			case PropertyMD::PropertyType::Int:
				Raw::DrawPropertyInt(elementName, currentElement, prop.m_componentCount, prop.m_draggable);
				break;
			case PropertyMD::PropertyType::Object: {
				DrawPropertyObject(currentElement, prop, prop.m_name);
				break;	
			}
			case PropertyMD::PropertyType::ResourceHandle:
				DrawPropertyResourceHandle(currentElement, prop, prop.m_name);
				break;

			default:
				break;
			}
			
		}
		ImGui::EndTable();
	}

}


