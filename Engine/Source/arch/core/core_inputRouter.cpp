#include <arch/core/core_inputRouter.h>

void InputRouter::RegisterSystemInternal(InputClient&& _inputClient) {
	if (SystemExists(_inputClient.m_clientName)) return;
	m_inputClientRegistry.insert({ _inputClient.m_clientName, _inputClient});
}

void InputRouter::BeginFrame() {
	UpdateOwnership();
	
	// update alive frame
	for (auto it = m_inputClientPriority.begin(); it != m_inputClientPriority.end(); ) {
		if (!it->m_aliveThisFrame) {
			m_priorityLookup.erase(it->m_clientName);
			it = m_inputClientPriority.erase(it);
		}
		else {
			// reset for next frame
			const_cast<InputPriorityNode&>(*it).m_aliveThisFrame = false;
			++it;
		}
	}
}

void InputRouter::UpdateOwnership() {
	std::optional<std::string> activeClientName = GetCurrentOwner();
	if (activeClientName != std::nullopt) {
		for (auto& [name, client] : m_inputClientRegistry) {
			bool enableInput{ name == activeClientName };
			//LOG_INFO((enableInput ? "Enabling" : "Disabling") << " Input for " << name);
			client.EnableAllInputs(name == activeClientName);
		}
	}
}


bool InputRouter::SystemExists(std::string _inputClientName) {
	return m_inputClientRegistry.find(_inputClientName) != m_inputClientRegistry.end();
}

void InputRouter::RequestOwnership(std::string _client, int _priority) {
	if (!SystemExists(_client)) return;

	auto it = m_priorityLookup.find(_client);

	if (it != m_priorityLookup.end()) {
		// Update existing node
		auto nodeIt = it->second;
		InputPriorityNode updated = *nodeIt;

		updated.m_priority = std::max(updated.m_priority, _priority);
		updated.m_aliveThisFrame = true;

		m_inputClientPriority.erase(nodeIt);
		auto newIt = m_inputClientPriority.insert(updated).first;
		m_priorityLookup[_client] = newIt;
	}
	else {
		// New request
		InputPriorityNode node{ _client, _priority, true };
		auto newIt = m_inputClientPriority.insert(node).first;
		m_priorityLookup[_client] = newIt;
	}
}


void InputRouter::ReleaseOwnership(std::string _client) {
	if (!SystemExists(_client)) return;
	InputClient& client{ m_inputClientRegistry.at(_client) };
	client.EnableAllInputs(false);
	m_inputClientPriority.erase(m_priorityLookup.at(_client));
	m_priorityLookup.erase(_client);
	
}

void InputRouter::ReleaseAllOwnership() {
	// duplicate to a different set to prevent access errors
	std::set<InputPriorityNode, InputPriorityNodeCompare> duplicateSet { m_inputClientPriority };


	for (const InputPriorityNode& inputPriority : duplicateSet) {
		std::string clientName	{ inputPriority.m_clientName };
		ReleaseOwnership(clientName);
	}
}

std::optional<std::string> InputRouter::GetCurrentOwner() {
	if (!m_inputClientPriority.size()) return std::nullopt;
	return m_inputClientPriority.begin()->m_clientName;
}

std::vector<std::string> InputRouter::GetAllClients() {
	std::vector<std::string> names;
	names.reserve(m_inputClientRegistry.size());
	for (const auto& [name, _] : m_inputClientRegistry) {
		names.push_back(name);
	}
	return names;
}
