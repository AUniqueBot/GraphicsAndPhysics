#pragma once
#include <pch.h>
#include <functional>



template <typename FunctionSignature> // ... forward 
class EventSystem;

template <typename F, typename... Args> // ... forward 
class EventSystem<F(Args...)> {

	using FunctionType = std::function<F(Args...)>;

public:

	void Dispatch(Args... args) {
		for (auto& [id, function] : m_subscribedFunctions) {
			if (function)
				function(args...);
		}
	}

	// returns the id of the subscribed function. used for subscription
	unsigned Subscribe(FunctionType _fn) {
		const unsigned id = !m_availableIds.empty() ? m_availableIds.front() : m_nextId++;
		if (!m_availableIds.empty()) m_availableIds.pop();
		m_subscribedFunctions.emplace(id, std::move(_fn));
		return id;
	}

	void Unsubscribe(unsigned _id) {
		if (m_subscribedFunctions.erase(_id)) {
			m_availableIds.push(_id);
		}
	}

	void Clear() {
		m_subscribedFunctions.clear();
		m_nextId = 0;
		while (!m_availableIds.empty()) m_availableIds.pop();
	}

private:
	std::unordered_map<unsigned, FunctionType> m_subscribedFunctions;
	std::queue<unsigned> m_availableIds;
	unsigned m_nextId{};
};


