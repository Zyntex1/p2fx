#pragma once
#include "Features/Feature.hpp"
#include "Variable.hpp"

class TimerAverage;
class TimerCheckPoints;

class Timer : public Feature {
public:
	bool isRunning;
	bool isPaused;
	int baseTick;
	int totalTicks;
	TimerAverage *avg;
	TimerCheckPoints *cps;

public:
	Timer();
	~Timer();
	void Start(int engineTick);
	void Rebase(int engineTick);
	int GetTick(int engineTick);
	void Save(int engineTick);
	void Stop(int engineTick);
};

extern Timer *timer;

extern Variable p2fx_timer_always_running;
extern Variable p2fx_timer_time_pauses;
