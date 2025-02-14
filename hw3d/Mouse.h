#pragma once

#include <queue>

class Mouse
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			LPress = 0,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Invalid,
			MAX = 8
		};
	private:
		Type type;
		bool bIsLeftPressed;
		bool bIsRightPressed;
		int x;
		int y;
	public:
		Event()
			:
			type(Type::Invalid),
			bIsLeftPressed(false),
			bIsRightPressed(false),
			x(0),
			y(0)
		{}
		Event(Type type, const Mouse& parent) noexcept
			:
			type(type),
			bIsLeftPressed(parent.bIsLeftPressed),
			bIsRightPressed(parent.bIsRightPressed),
			x(parent.x),
			y(parent.y)
		{}
		bool IsValid() const noexcept { return type != Type::Invalid; }
		Type GetType() const noexcept { return type; }
		std::pair<int, int> GetPos() const noexcept { return std::pair{ x, y }; }
		int GetPosX() const noexcept { return x; }
		int GetPosY() const noexcept { return y; }
		bool IsLeftPressed() const noexcept { return bIsLeftPressed; }
		bool IsRightPressed() const noexcept { return bIsRightPressed; }
	};

public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	std::pair<int, int> GetPos() const noexcept;
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
	bool IsLeftPressed() const noexcept;
	bool IsRightPressed() const noexcept;
	Mouse::Event Read() noexcept;
	bool IsEmpty() const noexcept
	{
		return mouseBuffer.empty();
	}
	void Flush() noexcept;

private:
	void OnMouseMove(int x, int y) noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void TrimBuffer() noexcept;

private:
	static constexpr unsigned int bufferSize = 16u;
	int x;
	int y;
	bool bIsLeftPressed;
	bool bIsRightPressed;
	std::queue<Event> mouseBuffer;
};