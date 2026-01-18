#pragma once
#include <pch.h>
#include <functional>
/*
	input clients can ask for 3 different outputs whenever, just need to specify which is needed.
*/

using GETTER_FN = std::function<bool()>;
using SETTER_FN = std::function<void(bool)>;

struct GetSetFunctions {
	GETTER_FN m_getter;
	SETTER_FN m_setter;
};

template <typename T>
GetSetFunctions GenerateFunctionPointers(
	T& _object, 
	bool (T::*_getter)() const, 
	void (T::*_setter)(bool)
) {
	return {
		[&_object, _getter]() { return _object.*_getter(); },
		[&_object, _setter](bool _v) { _object.*_setter(_v); }
	};
}




struct Interface_InputSystem {
	GETTER_FN isMouseEnabled;
	GETTER_FN isKeyboardEnabled;
	GETTER_FN isControllerEnabled;
	SETTER_FN setMouseEnabled;
	SETTER_FN setKeyboardEnabled;
	SETTER_FN setControllerEnabled;
};

struct InputClient {
	std::string m_clientName;
	Interface_InputSystem m_inputFunctions;
	bool HasFunctions() const {
		return
			m_inputFunctions.isControllerEnabled ||
			m_inputFunctions.setControllerEnabled ||
			m_inputFunctions.isKeyboardEnabled ||
			m_inputFunctions.setKeyboardEnabled ||
			m_inputFunctions.isMouseEnabled ||
			m_inputFunctions.setMouseEnabled
			;
	}

	void EnableAllInputs(bool _setting) const {
		if (m_inputFunctions.setControllerEnabled) m_inputFunctions.setControllerEnabled(_setting);
		if (m_inputFunctions.setKeyboardEnabled) m_inputFunctions.setKeyboardEnabled(_setting);
		if (m_inputFunctions.setMouseEnabled) m_inputFunctions.setMouseEnabled(_setting);
	}

};

struct InputPriorityNode {
	std::string m_clientName;
	int m_priority;
	bool m_aliveThisFrame;
	bool m_suspendedThisFrame;
};

struct InputPriorityNodeCompare {
	bool operator()(const InputPriorityNode& a, const InputPriorityNode& b) const {
		if (a.m_priority != b.m_priority)
			return a.m_priority > b.m_priority; // higher first
		return a.m_clientName < b.m_clientName; // tie-breaker (stable!)
	}
};

class InputRouter {
public:

	void BeginFrame();
	void UpdateOwnership();
	template <typename T>
	void RegisterSystem(T& _object, std::string _name = "");

	bool SystemExists(std::string _inputClientName);


	void RequestOwnership(std::string _client, int _priority); // from, replacement.
	void ReleaseOwnership(std::string _client); // from, replacement.

	void ReleaseAllOwnership();

	std::optional<std::string> GetCurrentOwner();

	std::vector<std::string> GetAllClients();

private:
	void RegisterSystemInternal(InputClient&& _inputClient);
private:
	std::unordered_map<std::string, InputClient> m_inputClientRegistry;
	std::set<InputPriorityNode, InputPriorityNodeCompare> m_inputClientPriority;
	std::unordered_map<std::string, std::set<InputPriorityNode>::iterator> m_priorityLookup;

};

// ---------------------------------------------------------------------------------------------

// template shenanigans

// ---------------------------------------------------------------------------------------------

template <typename T>
concept HasIsKeyboardEnabled =
	requires(T t) {
		{ t.AllowKeyboardInput() } -> std::convertible_to<bool>;
};

template <typename T>
concept HasSetKeyboardEnabled =
	requires(T t, bool v) {
		{ t.AllowKeyboardInput(v) } -> std::same_as<void>;
};

template <typename T>
concept HasIsMouseEnabled =
	requires(T t) {
		{ t.AllowMouseInput() } -> std::convertible_to<bool>;
};

template <typename T>
concept HasSetMouseEnabled =
	requires(T t, bool v) {
		{ t.AllowMouseInput(v) } -> std::same_as<void>;
};

template <typename T>
concept HasIsControllerEnabled =
	requires(T t) {
		{ t.AllowControllerInput() } -> std::convertible_to<bool>;
};

template <typename T>
concept HasSetControllerEnabled =
	requires(T t, bool v) {
		{ t.AllowControllerInput(v) } -> std::same_as<void>;
};



template <typename T>
void InputRouter::RegisterSystem(T& client, std::string _name) {
	InputClient ic{};
	ic.m_clientName = _name.length() ? _name: typeid(T).name(); // or provided name

	// Keyboard
	if constexpr (HasIsKeyboardEnabled<T> && HasSetKeyboardEnabled<T>) {
		ic.m_inputFunctions.isKeyboardEnabled =
			[&client]() { return client.AllowKeyboardInput(); };

		ic.m_inputFunctions.setKeyboardEnabled =
			[&client](bool v) { client.AllowKeyboardInput(v); };
	}

	// Mouse
	if constexpr (
		requires(T t) { t.AllowMouseInput(); }&&
		requires(T t, bool v) { t.AllowMouseInput(v); }
		) {
		ic.m_inputFunctions.isMouseEnabled =
			[&client]() { return client.AllowMouseInput(); };

		ic.m_inputFunctions.setMouseEnabled =
			[&client](bool v) { client.AllowMouseInput(v); };
	}

	// Controller (optional)
	if constexpr (
		requires(T t) { t.AllowControllerInput(); }&&
		requires(T t, bool v) { t.AllowControllerInput(v); }
		) {
		ic.m_inputFunctions.isControllerEnabled =
			[&client]() { return client.AllowControllerInput(); };

		ic.m_inputFunctions.setControllerEnabled =
			[&client](bool v) { client.AllowControllerInput(v); };
	}

	// if ALL 6 have no pointer, ignore
	if (!ic.HasFunctions()) return;
	RegisterSystemInternal(std::move(ic));
}