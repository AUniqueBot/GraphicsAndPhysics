#include <arch/core/core_gamestatemanager.h>


void GameStateManager::Update() {
	if (m_currentState == m_nextState) return; // do nothing.
	
	switch (m_nextState) {
	case GameState::Play:
		break;
	case GameState::Pause:
		break;
	case GameState::Stop:
		break;
	}
	m_currentState = m_nextState;
}

void GameStateManager::SetNextState(const GameState& _nextState) {
	m_nextState = _nextState;
}

const GameStateManager::GameState& GameStateManager::GetNextState() const {
	return m_nextState;
}

const GameStateManager::GameState& GameStateManager::GetCurrentState() const {
	return m_currentState;
}

void GameStateManager::ForceStop() {

}
