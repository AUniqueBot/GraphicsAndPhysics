
#include <UI_Core.h>
#include <Widgets/UIWidget_Inspector.h>
#include <imgui/misc/cpp/imgui_stdlib.h>



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

	//ImGui::
	auto trsV = obj.GetComponent<Transform>();
	if (trsV) {
		std::string tableId{ obj.Name() + "##Transform_Table" };
		Transform& trs = *trsV;
		
		
		bool tableInit = ImGui::BeginTable(tableId.c_str(), 2);
		if (tableInit) {

			float posBuffer[3]{
				trs.Position().x,
				trs.Position().y,
				trs.Position().z,
			};
			float rotBuffer[4]{
				trs.Rotation().w,
				trs.Rotation().x,
				trs.Rotation().y,
				trs.Rotation().z,
			};
			float sclBuffer[3]{
				trs.Scale().x,
				trs.Scale().y,
				trs.Scale().z,
			};
		
		
					
			TableNextRow();
			TableSetColumnIndex(0);
			if (InputFloat3("Position##Transform", posBuffer) && EnterOrTabPressed()) {
				trs.Position(glm::vec3(posBuffer[0], posBuffer[1], posBuffer[2]));
			}


			TableNextRow();
			TableSetColumnIndex(0);
			if (InputFloat4("Rotation##Transform", rotBuffer) && EnterOrTabPressed()) {
				trs.Rotation(glm::quat(rotBuffer[0], rotBuffer[1], rotBuffer[2], rotBuffer[3]));
			}

			TableNextRow();
			TableSetColumnIndex(0);
			if (InputFloat3("Scale##Transform", sclBuffer) && EnterOrTabPressed()) {
				trs.Scale(glm::vec3(sclBuffer[0], sclBuffer[1], sclBuffer[2]));
			}

			EndTable();
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
		// dropdown list.
	

		



	}

	auto camV = obj.GetComponent<Camera>();
	if (camV) {

	}


	auto lightV = obj.GetComponent<Light>();
	if (lightV) {
		Light& light = *lightV;
		

		float colBuffer[3] {
			light.Color().x,
			light.Color().y,
			light.Color().z,
		};
		if (ColorEdit3("Colour##Light", colBuffer)) {
			light.Color(glm::vec3(colBuffer[0], colBuffer[1], colBuffer[2]));
		}

		float pwrBuffer	{ light.Power() };
		if (InputFloat("Power##Light", &pwrBuffer)) {
			light.Power(pwrBuffer);
		}

		bool flag{light.GetCastShadow()};

		const char* currentLight{
			light.Type() == POINT ? "Point" :
			light.Type() == SPOT ? "Spot" :
			light.Type() == DIRECTIONAL ? "Directional":
			"Ambient"
		};

		if (ImGui::BeginCombo("Light Type", currentLight)) {
			if (ImGui::Selectable("Point")) {
				light.Type(POINT);
			}
			if (ImGui::Selectable("Directional")) {
				light.Type(DIRECTIONAL);
			}
			if (ImGui::Selectable("Ambient")) {
				light.Type(AMBIENT);
			}
			ImGui::EndCombo();
		}
		if (light.Type() != AMBIENT && Checkbox("Casts Shadow##Light", &flag)) {
			light.SetCastShadow(flag);
		}
	}

	
}


void UIWidget_Inspector::Exit() {

}

