#pragma once
#include <pch.h>

class Clock {
public:
	void Update();



	static double DeltaTime();
	static double FixedDeltaTime();
	static void FixedDeltaTime(double _dt);

	static unsigned FixedUpdateCount();
private:

	static void RefreshFixedUpdateCount();

private:
	static double m_deltaTime;
	static double m_fixedDeltaTime;
	static double m_accumulatedDeltaTime;
	static unsigned m_fixedUpdateCount;
	

};