#pragma once
#include <queue>
#include <bitset>
#include "Timer.h"
class Keyboard
{
	friend class Window;
public:
	class Event {
	public:
		enum class Type {
			Press,
			Release,
			Down,
			Invalid
		};
	private:
		Type type;
		unsigned char code;
	public:
		Event() :type(Type::Invalid), code(0u) {}
		Event(Type type, unsigned char code) noexcept : type(type), code(code) {}
		bool IsPressed() const noexcept {
			return type == Type::Press;
		}
		bool IsRelease() const noexcept {
			return type == Type::Release;
		}
		bool IsDown() const noexcept {
			return type == Type::Down;
		}
		bool IsInvalid() const noexcept {
			return type != Type::Invalid;
		}
		unsigned char GetCode() const noexcept {
			return code;
		};

	};
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	//key event
	bool KeyIsPressed(unsigned char code) const noexcept;
	bool KeyIsDown(unsigned char code) const noexcept;
	Event ReadKey() noexcept;
	bool KeyIsEmpty() const noexcept;
	void FlushKey() noexcept;
private:
	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void ClearState() noexcept;
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;
private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	bool autorepeatEnabled = false;
	std::bitset<nKeys> keystates;
	std::queue<Event> keybuffer;
	std::queue<char> charbuffer;
	Timer m_time;


};
