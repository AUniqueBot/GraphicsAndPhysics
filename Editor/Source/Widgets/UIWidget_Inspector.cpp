
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

	//static bool DrawPropertyPointer(const std::string& _label, void* _value, int, bool) {
	//	if (!_prop.m_get || !_prop.m_set) return;

	//	GetterFunction getter = _prop.m_get;
	//	Inspectable* handle{ GetValueFromGetter<Inspectable*>(getter, object) };
	//	if (handle == nullptr) return;

	//	for (auto& prop : handle->GetProperties()) {
	//		DrawPropertyElement((void*)handle, prop, prop.m_name);
	//	}
	//}

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





	UI_Core& uic = *puic;

	const UI_Selectable& selection = uic.SelectedItem();


	if (selection.m_type == UI_Selectable::GameObject) {
		DrawEntity();
	}
	else if (selection.m_type == UI_Selectable::Resource) {
		DrawResource();
	}


}

void UIWidget_Inspector::DrawEntity() {
	using namespace ImGui;
	Core& core = *ApplicationCore();
	EntityID selectedID = SelectedItem().m_id.m_entityId;
	EntityView selectedObj = core.GetRegistry().GetEntity(selectedID);
	if (!selectedObj) return;
	
	
	EntityRegistry& er = core.GetRegistry();
	ResourceManager& rsmgr = core.GetResourceManager();

	//Text("Object selected with ID [%lu]", selectedID);
	std::function<bool()> EnterOrTabPressed = []() {
		return
			ImGui::IsKeyPressed(ImGuiKey_Enter) ||
			ImGui::IsKeyPressed(ImGuiKey_Tab) ||
			ImGui::IsKeyPressed(ImGuiKey_KeypadEnter);
		};


	Entity& obj = *selectedObj;
	std::string s{ selectedObj->Name() };
	if (InputText("Object Name", &s) && EnterOrTabPressed()) {
		obj.Name(s);
	}

	Text("ID: [%lu]", obj.GetID());

	// render components here.


	for (ComponentHandle& compHandle : er.GetEntityComponents(selectedID)) {
		Component* comp{ compHandle.m_componentPtr };
		
		if (!comp) continue;
		ImGui::Separator();

		const std::vector<PropertyMD::Property>& props{ comp->GetProperties() };
		if (!props.size()) {
			continue;
		}
		std::string compName = compHandle.m_componentMetadata.GetComponentName();
		if (ImGui::CollapsingHeader(compName.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
			for (const PropertyMD::Property& prop : props) {
				ImGui::SeparatorText(prop.m_name.c_str());
				DrawPropertyElement(comp, prop, prop.m_name);
			}
		}

	}


	
	ImGui::Spacing();
	ImVec2 buttonSize(200.0f, 20.0f);
	float x = (ImGui::GetContentRegionAvail().x - buttonSize.x) * 0.5f;
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x);
	if (ImGui::Button("Add Component", buttonSize)) {
		ImGui::OpenPopup("Add Component##List");
	}
	if (ImGui::BeginPopup("Add Component##List")) {
		auto& compData = er.GetAllComponentData();
		for (auto& [key, val] : compData) {
			const std::string& compName = val.m_componentMetadata.GetComponentName();
			if (ImGui::Selectable(compName.c_str())) {
				// add the component directly to the thing.
				CompID compId = val.m_componentMetadata.GetComponentTypeID();
				er.AddComponent(selectedID, compId);
			}

		}
		ImGui::EndPopup();
	}
}

void UIWidget_Inspector::DrawResource() {
	using namespace ImGui;
	Core& core = *ApplicationCore();
	EntityRegistry& er = core.GetRegistry();
	ResourceManager& rsmgr = core.GetResourceManager();
	RES_ID selectedID = SelectedItem().m_id.m_resId;
	auto res = rsmgr.GetResource(selectedID);
	
	if (!res) {
		Text("No resource with ID [%i]", selectedID);
		return;
	}

	if (res->ResourceType() == ResourceConstants::C_RESTYPE_INVALID_ID) {
		return;
	}

	ResourceTypeMetadata metadata = rsmgr.GetResourceTypeMetadata(res->ResourceType());
	PushID(selectedID);
	
	if (CollapsingHeader(metadata.GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
		for (const PropertyMD::Property& prop : res->GetProperties()) {
			ImGui::SeparatorText(prop.m_name.c_str());
			DrawPropertyElement(res.get(), prop, prop.m_name);
		}
	}
	PopID();
}


void UIWidget_Inspector::Exit() {

}

void UIWidget_Inspector::PinTrackedItem() {
	m_trackedItem = UICore()->SelectedItem();
}

void UIWidget_Inspector::UnpinTrackedItem() {
	m_trackedItem = UI_Selectable();
}
bool UIWidget_Inspector::IsTrackingItem() const {
	return m_trackedItem.m_type != UI_Selectable::NONE;
}

UI_Selectable UIWidget_Inspector::SelectedItem() {
	return IsTrackingItem() ?  m_trackedItem : UICore()->SelectedItem();
}


// - individual elements --------------------------------

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
		DrawPropertyResourceHandle(object, prop, name, true);
		break;
	default:
		break;
	}

}


void UIWidget_Inspector::DrawPropertyInt(void* object, const PropertyMD::Property& prop, const std::string& key) {
	
	GetterFunction getter = prop.m_get;
	SetterFunction setter = prop.m_set;
	switch (prop.m_componentCount) {

	case 1: {
		int val{ GetValueFromGetter<int>(getter, object) };
		if (Raw::DrawPropertyInt(key, &val, 1, prop.m_draggable))
			prop.m_set(object, &val);
		break;
	}

	case 2: {
		glm::ivec2 val{ GetValueFromGetter<glm::ivec2>(getter, object) };
		if (Raw::DrawPropertyInt(key, glm::value_ptr(val), 2, prop.m_draggable))
			prop.m_set(object, &val);
		break;
	}

	case 3: {
		glm::ivec3 val{ GetValueFromGetter<glm::ivec3>(getter, object) };
		if (Raw::DrawPropertyInt(key, glm::value_ptr(val), 3, prop.m_draggable))
			prop.m_set(object, &val);
		break;
	}

	case 4: {
		glm::ivec4 val{ GetValueFromGetter<glm::ivec4>(getter, object) };
		if (Raw::DrawPropertyInt(key, glm::value_ptr(val), 4, prop.m_draggable))
			prop.m_set(object, &val);

		break;
	}
	}
}


void UIWidget_Inspector::DrawPropertyFloat(void* object, const PropertyMD::Property& prop, const std::string& key) {
	GetterFunction getter = prop.m_get;
	SetterFunction setter = prop.m_set;
	switch (prop.m_componentCount) {

	case 1: {
		float val{ GetValueFromGetter<float>(getter, object) };
		if (Raw::DrawPropertyFloat(key, &val, 1, prop.m_draggable))
			prop.m_set(object, &val);
		break;
	}

	case 2: {
		glm::vec2 val{ GetValueFromGetter<glm::vec2>(getter, object) };
		if (Raw::DrawPropertyFloat(key, glm::value_ptr(val), 2, prop.m_draggable))
			prop.m_set(object, &val);
		break;
	}

	case 3: {
		glm::vec3 val{ GetValueFromGetter<glm::vec3>(getter, object) };
		if (Raw::DrawPropertyFloat(key, glm::value_ptr(val), 3, prop.m_draggable))
			prop.m_set(object, &val);
		break;
	}

	case 4: {
		glm::vec4 val{ GetValueFromGetter<glm::vec4>(getter, object) };
		if (Raw::DrawPropertyFloat(key, glm::value_ptr(val), 4, prop.m_draggable))
			prop.m_set(object, &val);
		break;
	}
	}
}


void UIWidget_Inspector::DrawPropertyDouble(void* object, const PropertyMD::Property& prop, const std::string& key) {

	GetterFunction getter = prop.m_get;
	SetterFunction setter = prop.m_set;
	double val{ GetValueFromGetter<double>(getter, object) };
	if (Raw::DrawPropertyDouble(key, &val, 1, false))
		prop.m_set(object, &val);
}


void UIWidget_Inspector::DrawPropertyColor(void* object, const PropertyMD::Property& prop, const std::string& key) {

	GetterFunction getter = prop.m_get;
	SetterFunction setter = prop.m_set;

	if (prop.m_componentCount == 3) {
		glm::vec3 val{ GetValueFromGetter<glm::vec3>(getter, object) };
		if (Raw::DrawPropertyColor(key, glm::value_ptr(val), 3, false))
			prop.m_set(object, &val);
	}
	else {
		glm::vec4 val{ GetValueFromGetter<glm::vec4>(getter, object) };

		if (Raw::DrawPropertyColor(key, glm::value_ptr(val), 4, false))
			prop.m_set(object, &val);
	}
}


void UIWidget_Inspector::DrawPropertyBoolean(void* object, const PropertyMD::Property& prop, const std::string& key) {
	GetterFunction getter = prop.m_get;
	SetterFunction setter = prop.m_set;
	bool val{ GetValueFromGetter<bool>(getter, object) };
	if (Raw::DrawPropertyBoolean(key, &val, 1, false))
		prop.m_set(object, &val);
}


void UIWidget_Inspector::DrawPropertyString(void* object, const PropertyMD::Property& prop, const std::string& key) {
	GetterFunction getter = prop.m_get;
	SetterFunction setter = prop.m_set;
	std::string val{ GetValueFromGetter<std::string>(getter, object) };

	if (Raw::DrawPropertyString(key, &val, 1, false))
		prop.m_set(object, &val);
}


void UIWidget_Inspector::DrawPropertyOptions(void* object, const PropertyMD::Property& prop, const std::string& key) {
 	GetterFunction getter = prop.m_get;
	void* valPtr{ };
	getter(object, valPtr);
	int val = *(int*)valPtr;
	const char* currentOption{"INVALID"};
	int newVal = val;
	// search id.
	for (const PropertyMD::Option& option : prop.m_options) {
		if (val == option.value) {
			currentOption = option.label;
			break;
		}
	}
	if (ImGui::BeginCombo(key.c_str(), currentOption)) {
		for (const PropertyMD::Option& option: prop.m_options) {
			if (ImGui::Selectable(option.label, val == option.value)) {
				newVal = option.value;
			}
		}
		if (val != newVal) { 
			prop.m_set(object, &newVal);
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

void UIWidget_Inspector::DrawPropertyResourceCombo(void* object, const PropertyMD::Property& prop, const std::string& key) {
	// get the correct asset manager.
	
	auto managerView = ApplicationCore()->GetAssetManager().GetManager(prop.m_resourceType);
	if (!managerView) {
		return;
	}
	SpecializedManager manager = *managerView;
	// do something.
	const std::unordered_set<RES_ID>& resIdPool = manager->GetResourcePool();
	ResourceManager& resMgr = ApplicationCore()->GetResourceManager();
	
	GetterFunction getter = prop.m_get;
	ResourceHandle handle{ GetValueFromGetter<ResourceHandle>(getter, object) };
	ResourceHandle selectedResource { handle };
	

	std::shared_ptr<BaseResource> resPtr = handle.GetBaseResource();
	std::string currentResName = resPtr ?  resPtr->Name() : "INVALID_ID";

	if (ImGui::BeginCombo(prop.m_name.c_str(), currentResName.c_str())) {
		for (const RES_ID& resid : resIdPool) {
			std::string name = resMgr.GetResource(resid)->Name();
			ImGui::PushID((int)resid);
			if (ImGui::Selectable(name.c_str())) {
				selectedResource = ResourceHandle(resMgr.GetResourceIdentifier(resid));
			}
			ImGui::PopID();
		}
		ImGui::EndCombo();
	}
	if (selectedResource != handle) {
		prop.m_set(object, &selectedResource);
	}

	

}

void UIWidget_Inspector::DrawPropertyPointer(void* object, const PropertyMD::Property& _prop, const std::string& _key) {
	if (!_prop.m_get || !_prop.m_set) return;

	GetterFunction getter = _prop.m_get;
	Inspectable* handle{ GetValueFromGetter<Inspectable*>(getter, object) };
	if (handle == nullptr) return;

	for (auto& prop : handle->GetProperties()) {
		DrawPropertyElement((void*)handle, prop, prop.m_name);
	}


}

void UIWidget_Inspector::DrawPropertyResourceHandle(
	void* object,
	const PropertyMD::Property& prop,
	const std::string& key,
	bool _drawCombo
) {
	if (!prop.m_get || !prop.m_set) return;

	GetterFunction getter = prop.m_get;
	ResourceHandle handle{ GetValueFromGetter<ResourceHandle>(getter, object) };

	if (!handle.HandleIsValid()) return;
	std::shared_ptr<BaseResource> res = handle.GetBaseResource();
	if (!res) return;
	for (auto& prop : res->GetProperties()) {
		DrawPropertyElement((void*)res.get(), prop, prop.m_name);
	}
	if (_drawCombo) {
		DrawPropertyResourceCombo(object, prop, key);
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
	
	
	if (ImGui::BeginTable(name.c_str(), 3, flags)) {
		auto& list = prop.m_list;
		size_t size = static_cast<size_t>(list.m_size(object));
		
		ImGui::TableSetupColumn("##Index", ImGuiTableColumnFlags_WidthFixed, 40.0f);
		ImGui::TableSetupColumn("Element", ImGuiTableColumnFlags_WidthStretch, 50.f);
		ImGui::TableSetupColumn("##Util", ImGuiTableColumnFlags_WidthStretch, 10.f);
		ImGui::TableHeadersRow();
		
		
		size_t deleted = 0;
		for (size_t i{}; i < size; ++i) {
			ImGui::PushID((int)i);
			
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%i", i);
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
			case PropertyMD::PropertyType::Pointer: {
				Inspectable* handle{ static_cast<Inspectable*>(currentElement) };
				if (handle == nullptr) return;
				std::string suffix = elementName;
				for (auto& prop : handle->GetProperties()) {
					DrawPropertyElement((void*)handle, prop, prop.m_name + suffix);
				}


				break;
			}

			case PropertyMD::PropertyType::ResourceHandle: {

				// if the object is the resource handle, you'll need the getters and setters

				ResourceHandle* handle = (ResourceHandle*)currentElement;
				if (!handle || !handle->HandleIsValid()) break;
				std::shared_ptr<BaseResource> res = handle->GetBaseResource();
				for (auto& prop : res->GetProperties()) {
					DrawPropertyElement((void*)res.get(), prop, prop.m_name);
				}

				break;

			}
			default:
				break;
			}


			ImGui::TableSetColumnIndex(2);
			std::string del = "Delete";
			del += elementName;
			if (ls.m_remove && ImGui::Button(del.c_str())) {
				// offset 1.
				deleted = i + 1;
			}
			ImGui::PopID();
		}


		if (deleted != 0) {
			ls.m_remove(object, (int)(deleted - 1));			
			deleted = 0;
		}
		ImGui::EndTable();
	}
	
	if (!ls.m_add) return; 
	std::string addLabel = "Add##List";
	addLabel += key;
	if (ls.m_type == PropertyMD::PropertyType::ResourceHandle) {
		// use a combo
		/*
			[RES NAME][add]
		*/

		auto managerView = ApplicationCore()->GetAssetManager().GetManager(prop.m_resourceType);
		if (!managerView) {
			return;
		}
		SpecializedManager manager = *managerView;
		const std::unordered_set<RES_ID>& resIdPool = manager->GetResourcePool();
		ResourceManager& resMgr = ApplicationCore()->GetResourceManager();

		// current resource.
		static ResourceHandle selectedResource{ std::nullopt };
		std::string currentResName = selectedResource.HandleIsValid() ? 
			selectedResource.GetName() : 
			"INVALID_ID";
		if (ImGui::BeginCombo(prop.m_name.c_str(), currentResName.c_str())) {
			for (const RES_ID& resid : resIdPool) {
				std::string name = resMgr.GetResource(resid)->Name();
				ImGui::PushID(resid);
				if (ImGui::Selectable(name.c_str())) {
					selectedResource = ResourceHandle(resMgr.GetResourceIdentifier(resid));
				}
				ImGui::PopID();
			}
			ImGui::EndCombo();
		}


		ImGui::SameLine();
		ImGui::BeginDisabled(!selectedResource.HandleIsValid());


		if (ImGui::Button(addLabel.c_str())) {
			ls.m_add(object, std::any(selectedResource));
		}
		ImGui::EndDisabled();

	}
	else if (ls.m_constructors.size()) {


		if (ImGui::Button(addLabel.c_str())) {
			if (ls.m_constructors.size() == 1) {
				// single element addition
				ls.m_add(object, ls.m_constructors[0].second());
			}
			else {
				// do a context menu
				ImGui::OpenPopup("##ListItems");
			}
		}

		if (ImGui::BeginPopup("##ListItems")) {
			for (auto& [label, fn] : ls.m_constructors) {
				if (ImGui::Selectable(label.c_str())) {
					ls.m_add(object, fn());
				}
			}
			ImGui::EndPopup();
		}
	}
	

}


