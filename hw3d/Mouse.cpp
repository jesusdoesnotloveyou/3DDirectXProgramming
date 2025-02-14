#include "Mouse.h"

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
    if (bufferSize > 0u)
    {
        Mouse::Event e = mouseBuffer.front();
        mouseBuffer.pop();
        return e;
    }
    else
    {
        return Mouse::Event{};
    }
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