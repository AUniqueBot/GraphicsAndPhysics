
#include <UI_Core.h>
#include <Widgets/UIWidget_Inspector.h>
#include <imgui_stdlib.h>


#include <arch/components/comp_transform.h>
#include <arch/components/comp_meshrenderer.h>
#include <arch/components/comp_light.h>
#include <arch/components/comp_camera.h>

#include <arch/resources/res_mesh_presets/res_mesh_types.h>


#include <arch/resources/res_material_presets/res_material_lambert.h>



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
		if (props.size()) {

			if (ImGui::CollapsingHeader(compHandle.m_componentMetadata->GetComponentName().c_str())) {
				for (const PropertyMD::Property& prop : props) {
					DrawPropertyElement(comp, prop, prop.m_name);
				}
			}
		}
		
	}




	auto meshV = obj.GetComponent<MeshRenderer>();
	if (meshV) {
		MeshRenderer& mr = *meshV;
		ResourceManager& resmgr = papc->GetResourceManager();
		auto& matList{ mr.GetMaterialList() };
		if (matList.size() == 0) {
			LambertMaterial& mat = *dynamic_cast<LambertMaterial*>(&mr.GetDefaultMaterial());
			glm::vec4 col{mat.Color()};
			if (ColorEdit4("Color##MeshRenderer", glm::value_ptr(col))) {
				mat.Color(col);
			}
		}

		else {

			// foreach loop, then typecast it.
			// need self-reflection.
			LambertMaterial& mat = *dynamic_cast<LambertMaterial*>(matList[0].get());

			// creating a call function for the component is the incorrect method about going through this.


			glm::vec4 col{mat.Color()};
			if (ColorEdit4("Color##MeshRenderer", glm::value_ptr(col))) {
				mat.Color(col);
			}
		}

		const auto& selectedMesh = mr.GetMesh();
		std::string selectedMeshName = selectedMesh->Name();

		RES_ID selectedMeshID = selectedMesh->ResourceID();
		if (BeginCombo("Mesh##Inspector_Meshes", selectedMeshName.c_str())) {
			const auto& resPool = resmgr.GetResourcePool();
			const auto& meshIDs = resmgr.GetResourcePoolManifest(Mesh::GetResourceTypeID());
			
			// first are the default meshes
			bool isSelected = false;
			for (unsigned i{}; i < Primitive::__COUNT; ++i) {

				std::shared_ptr<Mesh> mesh	{};
				std::string primitiveType	{};
				switch (i) {
					case Primitive::CUBE:
						mesh.reset(new Cube());
						primitiveType = "Cube";
						break;
					case Primitive::SPHERE:
						mesh.reset(new Sphere());
						primitiveType = "Sphere";
						break;
					case Primitive::PLANE:
						primitiveType = "Plane";
						mesh.reset(new Plane());
						break;
					case Primitive::ICOSPHERE:
						continue;
						primitiveType = "WIP - Icosphere";
						//mesh.reset(new Plane());
						break;
				}
				
				isSelected = selectedMeshName == primitiveType;
				if (Selectable(primitiveType.c_str())) {
					 mr.SetMesh(mesh);
				}
			}
			

			for (const RES_ID& id : meshIDs) {
				std::string imguiMeshID	{ "##meshID" };
				imguiMeshID += std::to_string(id);
				PushID(imguiMeshID.c_str());

				const auto& mesh = std::dynamic_pointer_cast<Mesh>(resPool.at(id));
				std::string name { mesh->ResourcePath().filename().string()  };

				isSelected = selectedMeshID == id;
				if (Selectable(name.c_str(), isSelected)) {
					mesh->Init();
					mr.SetMesh(mesh);
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
		return;
	case PropertyType::Int:
		DrawPropertyInt(object, prop, name);
		return;
	case PropertyType::Float:
		DrawPropertyFloat(object, prop, name);
		return;
	case PropertyType::Double:
		DrawPropertyDouble(object, prop, name);
		return;
	case PropertyType::Boolean:
		DrawPropertyBoolean(object, prop, name);
		return;
	case PropertyType::String:
		DrawPropertyString(object, prop, name);
		return;
	case PropertyType::Object:
		DrawPropertyObject(object, prop, name);
		break;
	default:
		break;
	}

}


void UIWidget_Inspector::DrawPropertyInt(void* object, const  PropertyMD::Property& prop, const std::string& key){
	int componentCount{ prop.m_componentCount };
	if (prop.m_draggable) {
		switch (componentCount) {
		case 1: {
			int val{};
			prop.m_get(object, &val);
			if (ImGui::DragInt(key.c_str(), &val)) {
				prop.m_set(object, &val);
			}
			break;
		}
		case 2: {
			glm::ivec2 val{};
			prop.m_get(object, &val);
			if (ImGui::DragInt2(key.c_str(), glm::value_ptr(val))) {
				prop.m_set(object, &val);
			}
			break;
		}
		case 3: {
			glm::ivec3 val{};
			prop.m_get(object, &val);
			if (ImGui::DragInt3(key.c_str(), glm::value_ptr(val))) {
				prop.m_set(object, &val);
			}
			break;
		}
		case 4: {
			glm::ivec4 val{};
			prop.m_get(object, &val);
			if (ImGui::DragInt4(key.c_str(), glm::value_ptr(val))) {
				prop.m_set(object, &val);
			}
			break;
		}
		}
	}
	else {
		switch (componentCount) {
		case 1: {
			int val{};
			prop.m_get(object, &val);
			if (ImGui::InputInt(key.c_str(), &val)) {
				prop.m_set(object, &val);
			}
			break;
		}
		case 2: {
			glm::ivec2 val{};
			prop.m_get(object, &val);
			if (ImGui::InputInt2(key.c_str(), glm::value_ptr(val))) {
				prop.m_set(object, &val);
			}
			break;
		}
		case 3: {
			glm::ivec3 val{};
			prop.m_get(object, &val);
			if (ImGui::InputInt3(key.c_str(), glm::value_ptr(val))) {
				prop.m_set(object, &val);
			}
			break;
		}
		case 4: {
			glm::ivec4 val{};
			prop.m_get(object, &val);
			if (ImGui::InputInt4(key.c_str(), glm::value_ptr(val))) {
				prop.m_set(object, &val);
			}
			break;
		}
		}
	}
}
void UIWidget_Inspector::DrawPropertyFloat(void* object, const PropertyMD::Property& prop, const std::string& key){
	int componentCount{ prop.m_componentCount };
	if (prop.m_draggable) {
		switch (componentCount) {
		case 1: {
			float val{};
			prop.m_get(object, &val);
			if (ImGui::DragFloat(key.c_str(), &val)) {
				prop.m_set(object, &val);
			}
			break;
		}
		case 2: {
			glm::vec2 val{};
			prop.m_get(object, &val);
			if (ImGui::DragFloat2(key.c_str(), glm::value_ptr(val))) {
				prop.m_set(object, &val);
			}
			break;
		}
		case 3: {
			glm::vec3 val{};
			prop.m_get(object, &val);
			if (ImGui::DragFloat3(key.c_str(), glm::value_ptr(val))) {
				prop.m_set(object, &val);
			}
			break;
		}
		case 4: {
			glm::vec4 val{};
			prop.m_get(object, &val);
			if (ImGui::DragFloat4(key.c_str(), glm::value_ptr(val))) {
				prop.m_set(object, &val);
			}
			break;
		}
		}
	}
	else {
		switch (componentCount) {
		case 1: {
			float val{};
			prop.m_get(object, &val);
			if (ImGui::InputFloat(key.c_str(), &val)) {
				prop.m_set(object, &val);
			}
			break;
		}
		case 2: {
			glm::vec2 val{};
			prop.m_get(object, &val);
			if (ImGui::InputFloat2(key.c_str(), glm::value_ptr(val))) {
				prop.m_set(object, &val);
			}
			break;
		}
		case 3: {
			glm::vec3 val{};
			prop.m_get(object, &val);
			if (ImGui::InputFloat3(key.c_str(), glm::value_ptr(val))) {
				prop.m_set(object, &val);
			}
			break;
		}
		case 4: {
			glm::vec4 val{};
			prop.m_get(object, &val);
			if (ImGui::InputFloat4(key.c_str(), glm::value_ptr(val))) {
				prop.m_set(object, &val);
			}
			break;
		}
		}
	}
}
void UIWidget_Inspector::DrawPropertyDouble(void* object, const PropertyMD::Property& prop, const std::string& key){
	double val{};
	prop.m_get(object, &val);

	if (ImGui::InputDouble(key.c_str(), &val)) {
		prop.m_set(object, &val);
	}

}


void UIWidget_Inspector::DrawPropertyColor(void* object, const  PropertyMD::Property& prop, const std::string& key){
	if (prop.m_componentCount == 3) {
		glm::vec3 val{};
		prop.m_get(object, &val);
		if (ImGui::ColorEdit3(key.c_str(), glm::value_ptr(val))) {
			prop.m_set(object, &val);
		}
	}
	else {
		glm::vec4 val{};
		prop.m_get(object, &val);
		if (ImGui::ColorEdit4(key.c_str(), glm::value_ptr(val))) {
			prop.m_set(object, &val);
		}
	}
}


void UIWidget_Inspector::DrawPropertyBoolean(void* object, const  PropertyMD::Property& prop, const std::string& key) {
	bool val{};
	prop.m_get(object, &val);
	if (ImGui::Checkbox(key.c_str(), &val)) {
		prop.m_set(object, &val);
	}
}
void UIWidget_Inspector::DrawPropertyString(void* object, const PropertyMD::Property& prop, const std::string& key){
	std::string val{};
	prop.m_get(object, &val);
	if (ImGui::InputText(key.c_str(), &val)) {
		prop.m_set(object, &val);
	}
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

void UIWidget_Inspector::DrawPropertyObject(void* object, const PropertyMD::Property& prop, const std::string& key) {
	// this object must provide a thing.
	void* val{};
	//prop.m_get(object, val);


	// assume the object provides this function.

	

}

void UIWidget_Inspector::DrawPropertiesDynamicList(void* object, const PropertyMD::Property& prop, const std::string& key) {
	// explain how it works.
	if (!prop.m_list.m_valid) return;

	void* val{};
	const PropertyMD::Property::List& ls{ prop.m_list };
	val = ls.m_listAccessor(object);

	val;

}
