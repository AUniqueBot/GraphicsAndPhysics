
#include <arch/core/core_clock.h>


double	Clock::m_deltaTime				{};
double	Clock::m_accumulatedDeltaTime	{};
double	Clock::m_fixedDeltaTime			{};
unsigned Clock::m_fixedUpdateCount		{};


void Clock::Update() {
	
	// delta time calculation
	static double previousTime{};
	double currentTime = glfwGetTime();
	m_deltaTime = currentTime - previousTime;
	previousTime = currentTime;


}

double Clock::DeltaTime() { return m_deltaTime; }
double Clock::FixedDeltaTime() { return m_fixedDeltaTime; }
void Clock::FixedDeltaTime(double _dt) {
	m_fixedDeltaTime = _dt;
	
}



unsigned Clock::FixedUpdateCount() {
	return m_fixedUpdateCount;
}


void Clock::RefreshFixedUpdateCount() {
	m_fixedUpdateCount = static_cast<unsigned>(m_deltaTime / m_fixedDeltaTime);
}


