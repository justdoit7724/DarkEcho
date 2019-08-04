#include "Timer.h"
#include "Debugging.h"

using namespace std::chrono;

int Timer::fps;
float Timer::spf;
std::chrono::time_point<std::chrono::steady_clock> Timer::prev;
std::chrono::time_point<std::chrono::steady_clock> Timer::first;

void Timer::Init()
{
	fps = 0;
	spf = 0;
	prev = high_resolution_clock::now();
	first = high_resolution_clock::now();
}

float fpsSec = 0;
void Timer::Update()
{
	auto curTime = high_resolution_clock::now();
	spf = duration<float, std::deca>(curTime - prev).count()*10.0f;
	prev = curTime;

	fpsSec += spf;

	if (fpsSec < 0.5f) // update fps per 0.5 sec
		return;
	fpsSec = 0;

	fps = 1.0f / spf;
}

float Timer::Elapsed()
{
	return duration<float, std::deca>(high_resolution_clock::now() - first).count()*10;
}

