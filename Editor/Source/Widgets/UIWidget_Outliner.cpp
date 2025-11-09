#include <Widgets/UIWidget_Outliner.h>
#include <arch/core.h>
#include <imgui.h>
#include <Widgets/UIWidget_Inspector.h>


#include <arch/components/comp_meshrenderer.h>
#include <arch/components/comp_transform.h>


UIWidget_Outliner::UIWidget_Outliner(std::string _widgetName) : UIWidget(_widgetName) {
    
}

UIWidget_Outliner::~UIWidget_Outliner() {
}

void UIWidget_Outliner::Init() {
    Core& c = Core::GetInstance(); // only on init.
    SetEntityRegistry(&c.Registry()); // setup aliases
    std::cout << "Initialisation Complete!\n";
}

void UIWidget_Outliner::Draw() const {
    using namespace ImGui;

    if (!m_entityRegistry) return;
    EntityRegistry& registry = *m_entityRegistry;

    if (CollapsingHeader("Outliner")) {
        Table();
    }
    if (CollapsingHeader("Add Objects")) {
        //
        if (Button("Add Cube")) {
            Entity& cubeObject = *registry.Instantiate();
            cubeObject.AddComponent<MeshRenderer>();
            cubeObject.Name("Cube");

            cubeObject.GetComponent<Transform>()->Position(glm::vec3(5, 5, 5));
        }
        

    }


}

void UIWidget_Outliner::Exit() {
    // -- cleanup -------------------------
    m_entityRegistry = nullptr;
}


void UIWidget_Outliner::OnSelect() {
    
}

void UIWidget_Outliner::OnUnselect() {

}

void UIWidget_Outliner::SetEntityRegistry(EntityRegistry* _registry) {
    if (_registry == m_entityRegistry) return;
    m_entityRegistry = _registry;
    LOG_INFO("Registry updated!");
}

void UIWidget_Outliner::Table() const {


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
    for (Entity& entity : registry.GetEntityList()) {
        TableNextRow();
        for (unsigned columnIdx{}; columnIdx < columns; ++columnIdx) {
            TableSetColumnIndex(columnIdx);
            switch (columnIdx) {
            case 0: {
                Text(entity.Name().c_str());
                break;
            }
            case 1: {
                std::string entityIsActiveStr = "##" + entity.Name() + "IsActive" + std::to_string(entity.GetID().GetID());
                bool entityIsActiveState{ entity.Active() };
                Checkbox(entityIsActiveStr.c_str(), &entityIsActiveState);
                entity.Active(entityIsActiveState);
                break;
            }
            case 2: {
                std::string entityIsVisibleStr = "##" + entity.Name() + "IsVisible" + std::to_string(entity.GetID().GetID());
                bool entityIsVisibleState{ entity.IsVisible() };
                Checkbox(entityIsVisibleStr.c_str(), &entityIsVisibleState);
                entity.IsVisible(entityIsVisibleState);
                break;
            }
            }
        }
    }



    ImGui::EndTable();
}


