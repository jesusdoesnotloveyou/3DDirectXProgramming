#include "Mouse.h"
#include <Windows.h>

std::pair<int, int> Mouse::GetPos() const noexcept
{
    return std::pair{x, y};
}

int Mouse::GetPosX() const noexcept
{
    return x;
}

int Mouse::GetPosY() const noexcept
{
    return y;
}

bool Mouse::IsInWindow() const noexcept
{
    return bIsInWindow;
}

bool Mouse::IsLeftPressed() const noexcept
{
    return bIsLeftPressed;
}

bool Mouse::IsRightPressed() const noexcept
{
    return bIsRightPressed;
}

Mouse::Event Mouse::Read() noexcept
{
    if (mouseBuffer.size() > 0u)
    {
        Mouse::Event e = mouseBuffer.front();
        mouseBuffer.pop();
        return e;
    }
    return Mouse::Event{};
}

void Mouse::Flush() noexcept
{
    mouseBuffer = std::queue<Event>{};
}

void Mouse::OnMouseMove(int newX, int newY) noexcept
{
    x = newX;
    y = newY;
    mouseBuffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
    TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept
{
    bIsInWindow = true;
    mouseBuffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
    TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept
{
    bIsInWindow = false;
    mouseBuffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
    TrimBuffer();
}

void Mouse::OnLeftPressed(int newX, int newY) noexcept
{
    bIsLeftPressed = true;
    mouseBuffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
    TrimBuffer();
}

void Mouse::OnLeftReleased(int newX, int newY) noexcept
{
    bIsLeftPressed = false;
    mouseBuffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
    TrimBuffer();
}

void Mouse::OnRightPressed(int newX, int newY) noexcept
{
    bIsRightPressed = true;
    mouseBuffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
    TrimBuffer();
}

void Mouse::OnRightReleased(int newX, int newY) noexcept
{
    bIsRightPressed = false;
    mouseBuffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
    TrimBuffer();
}

void Mouse::OnWheelUp(int newX, int newY) noexcept
{
    mouseBuffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
    TrimBuffer();
}

void Mouse::OnWheelDown(int newX, int newY) noexcept
{
    mouseBuffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
    TrimBuffer();
}

void Mouse::TrimBuffer() noexcept
{
    while (mouseBuffer.size() > bufferSize)
    {
        mouseBuffer.pop();
    }
}

void Mouse::OnWheelDelta(int x, int y, int delta) noexcept
{
    wheelDeltaCarry += delta;
    // generate events for every 120
    while (wheelDeltaCarry >= WHEEL_DELTA)
    {
        wheelDeltaCarry -= WHEEL_DELTA;
        OnWheelUp(x, y);
    }
    while (wheelDeltaCarry <= -WHEEL_DELTA)
    {
        wheelDeltaCarry += WHEEL_DELTA;
        OnWheelDown(x, y);
    }
}