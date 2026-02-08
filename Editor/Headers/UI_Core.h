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




	// -- inputs ---------------------------------------------------
	bool InputIsAllowed() const;
	void InputIsAllowed(bool _setting);

	bool AllowKeyboardInput() const;
	void AllowKeyboardInput(bool _setting);
	
	bool AllowMouseInput() const;
	void AllowMouseInput(bool _setting);




	Core* GetCore() const;



	inline GLuint GetIcon(std::string _name) const { return m_iconMap.at(_name); };

protected:
	void BeginDockSpace();

	void LoadIcons();


	void RenderTopBar() const;
	void RenderWidgets() const;

private:

	void UpdateAllowedInputs();
private:

	bool m_inputAllowed							{ true };
	bool m_keyboardInputAllowed					{ true };
	bool m_mouseInputAllowed					{ true };

	Core* m_applicationCore						{};

	std::unordered_map<std::string, GLuint> m_iconMap;


	std::unordered_map<std::string, std::shared_ptr<UIWidget>> m_widgetStorage;
	std::unordered_map<std::string, std::shared_ptr<UIMenu>> m_menuStorage;
	EntityRegistry* m_entityRegistry			{};
	std::vector<EntityID> m_selectedEntityList	{};
	
	
	mutable EntityID m_selectedEntity			{ EntityID::ENTITYID_INVALID };
};