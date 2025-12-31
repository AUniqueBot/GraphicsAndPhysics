#pragma once

#include <arch/core.h>
#include <arch/common/entityid.h>
#include <unordered_map>
#include <memory>

struct GLFWwindow;
class UIWidget;
class UIMenu;

class UI_Core {

	friend class UIWidget;

public:
	void Init(unsigned _major, unsigned _minor, GLFWwindow* _window, Core& _core);


	void Update();

	void Exit();

	// -- widget management ----------------------------------------
	std::string AddWidget(std::shared_ptr<UIWidget> _widget);
	void RemoveWidget(std::string _id);

	std::string AddMenuItem(std::shared_ptr<UIMenu> _menu);
	void RemoveMenuItem(std::string _id);


	// -- object selection -----------------------------------------
	EntityID SelectedEntity() const;

	Core* GetCore() const;

protected:
	void SelectedEntity(EntityID _id);



	void RenderTopBar() const;
	void RenderWidgets() const;

private:


private:
	Core* m_applicationCore						{};

	std::unordered_map<std::string, std::shared_ptr<UIWidget>> m_widgetStorage;
	std::unordered_map<std::string, std::shared_ptr<UIMenu>> m_menuStorage;
	EntityRegistry* m_entityRegistry			{};
	EntityID m_selectedEntity					{ EntityID::ENTITYID_INVALID };
	std::vector<EntityID> m_selectedEntityList	{};
};