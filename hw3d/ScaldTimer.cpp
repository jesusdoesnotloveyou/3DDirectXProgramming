#include "ScaldTimer.h"

ScaldTimer::ScaldTimer()
{
    last = _CHRONO steady_clock::now();
}

float ScaldTimer::Mark()
{
    const auto old = last;
    last = _CHRONO steady_clock::now();
    const _CHRONO duration<float> frameTime = last - old;

    return frameTime.count();
}

float ScaldTimer::Peek() const
{
    return _CHRONO duration<float>(_CHRONO steady_clock::now() - last).count();
}
