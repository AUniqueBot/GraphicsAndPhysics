#pragma once
#include <pch.h>


class GameStateManager {
	enum class GameState {
		Play,
		Pause,
		Stop
	};

public:
	void Update(); 

	void SetNextState(const GameState& _nextState);
	const GameState& GetNextState() const;
	const GameState& GetCurrentState() const;

	void ForceStop();

private:
	GameState m_nextState	 { GameState::Stop };
	GameState m_currentState { GameState::Stop };
};