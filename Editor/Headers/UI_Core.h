#pragma once

#include <arch/core.h>
#include <arch/common/entityid.h>
#include <unordered_map>
#include <memory>

struct GLFWwindow;
class UIWidget;

class UI_Core {

	friend class UIWidget;

public:
	void Init(unsigned _major, unsigned _minor, GLFWwindow* _window, Core& _core);


	void Update();

	void Exit();

	// -- widget management ----------------------------------------
	std::string AddWidget(std::shared_ptr<UIWidget> _widget);
	void RemoveWidget(std::string _id);


	// -- object selection -----------------------------------------
	EntityID SelectedEntity() const;
protected:
	void SelectedEntity(EntityID _id);

	Core* GetCore() const;


	void TopBar() const;


private:
	void DrawWidget(std::shared_ptr<UIWidget> _widget);

private:
	Core* m_applicationCore						{};

	std::unordered_map<std::string, std::shared_ptr<UIWidget>> m_widgetStorage;
	EntityRegistry* m_entityRegistry			{};
	EntityID m_selectedEntity					{ EntityID::ENTITYID_INVALID };
	std::vector<EntityID> m_selectedEntityList	{};
};