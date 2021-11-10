#pragma once

#include <array>
#include "glm/inc/glm.hpp"
#include "glfw/inc/glfw3.h"

class Input
{
public:

	struct KeyPressed : paperback::event::instance< int > {};
    struct MousePressed : paperback::event::instance< int > {};

	PPB_INLINE
	Input( paperback::coordinator::instance& Coordinator ) noexcept;

	PPB_INLINE
	~Input() = default;

	// Keyboard button state
	PPB_INLINE
	void SetKey(int Key, int Action) noexcept;

	// Check if key is pressed once
	PPB_INLINE
	bool IsKeyPress(int Key) const noexcept;
	// Check if key is held down
	PPB_INLINE
	bool IsKeyPressDown(int Key) const noexcept;
	// Check if key was release
	PPB_INLINE
	bool IsKeyPressUp(int Key) const noexcept;

	// Mouse button states
	PPB_INLINE
	void SetMouse(int Key, int Action) noexcept;
	PPB_INLINE
	void SetScroll(double Y) noexcept;

	// Check if button is press once
	PPB_INLINE
	bool IsMousePress(int Key) const noexcept;
	// Check if button is held down
	PPB_INLINE
	bool IsMouseDown(int Key) const noexcept;
	// Check if button was released
	PPB_INLINE
	bool IsMouseUp(int Key) const noexcept;

	// Check for scrolling
	PPB_INLINE
	bool IsScrollUp() noexcept;
	PPB_INLINE
	bool IsScrollDown() noexcept;

	// Return screen position of mouse
	PPB_INLINE
	glm::vec3 GetMousePosition() const noexcept;

	PPB_INLINE
	void UpateInputs() noexcept;

private:
	// State of key press
	struct Inputstate
	{
		int8_t m_Previous;
		int8_t m_Current;
	};

	// Array of keys inputs
	std::array<Inputstate, GLFW_KEY_LAST> m_Keys;

	// Array of mouse inputs
	std::array<Inputstate, GLFW_MOUSE_BUTTON_LAST> m_Buttons;

	// Offset of scroll wheel
	double m_YAxis;

	// Reference to Coordinator
	paperback::coordinator::instance& m_Coordinator;
};