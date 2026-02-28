#include <Widgets/UIWidget_Outliner.h>
#include <arch/core.h>
#include <Widgets/UIWidget_Inspector.h>


#include <arch/resources/res_mesh_presets/res_mesh_cube.h>
#include <arch/components/comp_meshrenderer.h>
#include <arch/components/comp_transform.h>
    
UIWidget_Outliner::UIWidget_Outliner(std::string _widgetName) 
    : UIWidget(_widgetName) {
    m_flags = {ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar};
}

UIWidget_Outliner::~UIWidget_Outliner() {
}

void UIWidget_Outliner::Init() {
    Core& c = Core::GetInstance(); // only on init.
    SetEntityRegistry(&c.GetRegistry()); // setup aliases
    SetFactory(&c.GetEntityFactory());
    LOG_INFO("Outliner Init.");
}

void UIWidget_Outliner::Draw() {
    using namespace ImGui;

    if (!m_entityRegistry) return;
    EntityRegistry& registry = *m_entityRegistry;
    Menu();
    //if (CollapsingHeader("Outliner")) {
    //}
    Table();
}

void UIWidget_Outliner::Exit() {
    // -- cleanup -------------------------
    m_entityRegistry = nullptr;
}


void UIWidget_Outliner::OnSelect() {
    
}

void UIWidget_Outliner::OnUnselect() {

}



void UIWidget_Outliner::Menu() {
    /* --------------------------------------------------------- */
    /*
        preview
        [primitive] [light] [type] [type]
    
    */
    /* --------------------------------------------------------- */

    if (!ImGui::BeginMenuBar()) return;

    //
    if (ImGui::BeginMenu("Primitives")) {
        if (ImGui::MenuItem("Cube")) {
            m_entityFactory->CreateCube();
        }
        if (ImGui::MenuItem("Sphere")) {
            m_entityFactory->CreateSphere();
        }
        if (ImGui::MenuItem("Plane")) {
            m_entityFactory->CreatePlane();
        }
        if (ImGui::MenuItem("Object")) {
            m_entityFactory->CreatePlane();
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Light")) {
        if (ImGui::MenuItem("Point Light")) {
            m_entityFactory->CreatePointLight();
        }
        if (ImGui::MenuItem("Directional Light")) {
            m_entityFactory->CreateDirectionalLight();
        }
        if (ImGui::MenuItem("Ambient Light")) {
            m_entityFactory->CreateAmbientLight();
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Other")) {
        if (ImGui::MenuItem("Game Object")) {
            m_entityFactory->CreateGameObject();
        }
        ImGui::MenuItem("Camera");
        ImGui::EndMenu();
    }


    ImGui::EndMenuBar();
}

void UIWidget_Outliner::SetEntityRegistry(EntityRegistry* _registry) {
    if (_registry == m_entityRegistry) return;
    m_entityRegistry = _registry;
    LOG_INFO("Registry updated!");
}

void UIWidget_Outliner::SetFactory(EntityFactory* _registry){
    if (_registry == m_entityFactory) return;
    m_entityFactory = _registry;
    LOG_INFO("Factory updated!");
}

void UIWidget_Outliner::Table() {


    /* --------------------------------------------------------- */
    /* 
        Preview of the table created

        +------------------------+-----------+------------+
        | Object Name            | Is Active | Is Visible |
        +------------------------+-----------+------------+
        |  objNameHere           |    [x]    |    [x]     |
        +------------------------+-----------+------------+
        .
        .
        .
        +------------------------+-----------+------------+

    */
    /* --------------------------------------------------------- */

    using namespace ImGui;
    std::string tableName = WidgetName() + "_list_" + "##" + std::to_string(WidgetID());
    const unsigned columns{ 3 };
    ImGuiTableFlags tableFlags{ ImGuiTableFlags_Borders | ImGuiTableFlags_Sortable | ImGuiTableFlags_Resizable };
    bool tableCreated = BeginTable(tableName.c_str(), columns, tableFlags);
    if (!tableCreated) {
        return;
    }
    TableSetupColumn("Object");
    TableSetupColumn("Active");
    TableSetupColumn("Visible");

    TableHeadersRow();
    // -- aliases -------------------------
    EntityRegistry& registry = *m_entityRegistry;

    Core& c = *ApplicationCore();
    for (Entity& entity : registry.GetEntityList()) {
        TableNextRow();

        ImGuiSelectableFlags selectableFlags =
            ImGuiSelectableFlags_::ImGuiSelectableFlags_SpanAllColumns |
            ImGuiSelectableFlags_::ImGuiSelectableFlags_AllowOverlap
            ;


        TableSetColumnIndex(0);

        std::string labelName = entity.Name() + "##";
        labelName += static_cast<unsigned long>(entity.GetID());
        ImGui::AlignTextToFramePadding();
        //float y{ ImGui::GetCursorPosY() };

        const bool clicked = Selectable(
            labelName.c_str(),
            c.GetRegistry().SelectedEntity() == entity.GetID(),
            selectableFlags,
            ImVec2{0, ImGui::GetFrameHeight()} // fill the entire row height
        );


        TableSetColumnIndex(1);
        std::string entityIsActiveStr = "##" + entity.Name() + "IsActive" + std::to_string(entity.GetID().GetID());
        bool entityIsActiveState{ entity.Active() };
        //ImGui::SetCursorPosY(y);
        Checkbox(entityIsActiveStr.c_str(), &entityIsActiveState);
        entity.Active(entityIsActiveState);



        TableSetColumnIndex(2);
        std::string entityIsVisibleStr = "##" + entity.Name() + "IsVisible" + std::to_string(entity.GetID().GetID());
        bool entityIsVisibleState{ entity.IsVisible() };
        //ImGui::SetCursorPosY(y);
        Checkbox(entityIsVisibleStr.c_str(), &entityIsVisibleState);
        entity.IsVisible(entityIsVisibleState);


        if (clicked) {
            // do something
            m_entityRegistry->SelectEntity(
                m_entityRegistry->SelectedEntity() == entity.GetID() ?
                EntityID::ENTITYID_INVALID : static_cast<unsigned long>(entity.GetID())
            );
        }
        
    }



    ImGui::EndTable();
}


