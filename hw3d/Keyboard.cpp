#include "Keyboard.h"

bool Keyboard::IsKeyPressed(unsigned char keyCode) const noexcept
{
    return keyStates[keyCode];
}

Keyboard::Event Keyboard::ReadKey() noexcept
{
    if (keyBuffer.size() > 0u)
    {
        Keyboard::Event e = keyBuffer.front();
        keyBuffer.pop();
        return e;
    }
    else
    {
        return Keyboard::Event{};
    }
}

bool Keyboard::IsKeyEmpty() const noexcept
{
    return keyBuffer.empty();
}

void Keyboard::FlushKey() noexcept
{
    keyBuffer = std::queue<Event>{};
}

char Keyboard::ReadChar() noexcept
{
    if (charBuffer.size() > 0u)
    {
        unsigned char charCode = charBuffer.front();
        charBuffer.pop();
        return charCode;
    }
    else
    {
        return 0;
    }
}

bool Keyboard::IsCharEmpty() const noexcept
{
    return charBuffer.empty();
}

void Keyboard::FlushChar() noexcept
{
    charBuffer = std::queue<char>{};
}

void Keyboard::Flush() noexcept
{
    FlushKey();
    FlushChar();
}

void Keyboard::EnableAutorepeat() noexcept
{
    autorepeatEnabled = true;
}

void Keyboard::DisableAutorepeat() noexcept
{
    autorepeatEnabled = false;
}

bool Keyboard::IsAutorepeatEnabled() const noexcept
{
    return autorepeatEnabled;
}

void Keyboard::OnKeyPressed(unsigned char keyCode) noexcept
{
    keyStates[keyCode] = true;
    keyBuffer.push(Keyboard::Event{ Keyboard::Event::Type::Press, keyCode });
    TrimBuffer(keyBuffer);
}

void Keyboard::OnKeyReleased(unsigned char keyCode) noexcept
{
    keyStates[keyCode] = false;
    keyBuffer.push(Keyboard::Event{ Keyboard::Event::Type::Release, keyCode });
    TrimBuffer(keyBuffer);
}

void Keyboard::OnChar(char character) noexcept
{
    charBuffer.push(character);
    TrimBuffer(charBuffer);
}

void Keyboard::ClearState() noexcept
{
    keyStates.reset();
}