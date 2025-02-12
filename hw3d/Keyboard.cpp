#include "Keyboard.h"

bool Keyboard::IsKeyPressed(unsigned char keyCode) const noexcept
{
    return false;
}

Keyboard::Event Keyboard::ReadKey() noexcept
{
    return Event();
}

bool Keyboard::IsKeyEmpty() const noexcept
{
    return false;
}

void Keyboard::FlushKey() noexcept
{
}

char Keyboard::ReadChar() noexcept
{
    return 0;
}

bool Keyboard::IsCharEmpty() const noexcept
{
    return false;
}

void Keyboard::FlushChar() noexcept
{
}

void Keyboard::Flush() noexcept
{
}

void Keyboard::EnableAutorepeat() noexcept
{
}

void Keyboard::DisableAutorepeat() noexcept
{
}

void Keyboard::IsAutorepeatEnabled() const noexcept
{
}

void Keyboard::OnKeyPressed(unsigned char keyCode) noexcept
{
}

void Keyboard::OnKeyReleased(unsigned char keyCode) noexcept
{
}

void Keyboard::OnChar(char character) noexcept
{
}

void Keyboard::ClearState() noexcept
{
}
