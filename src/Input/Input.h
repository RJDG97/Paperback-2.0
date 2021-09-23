#pragma once

#include <array>
#include "glm/inc/glm.hpp"
#include "glfw/inc/glfw3.h"

class Input
{
public:
	~Input() = default;

	// Keyboard button state
	void SetKey(int Key, int Action);

	// Check if key is pressed once
	bool IsKeyPress(int Key) const;
	// Check if key is held down
	bool IsKeyPressDown(int Key) const;
	// Check if key was release
	bool IsKeyPressUp(int Key) const;

	// Mouse button states
	void SetMouse(int Key, int Action);
	void SetScroll(double Y);

	// Check if button is press once
	bool IsMousePress(int Key);
	// Check if button is held down
	bool IsMouseDown(int Key) const;
	// Check if button was released
	bool IsMouseUp(int Key);

	// Check for scrolling
	bool IsScrollUp();
	bool IsScrollDown();

	// Return screen position of mouse
	glm::vec3 GetMousePosition() const;

	void UpateInputs();

	// Singleton
	static Input& GetInstance();
	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

private:
	// State of key press
	struct Inputstate
	{
		int8_t m_Previous;
		int8_t m_Current;
	};

	Input();

	// Array of keys inputs
	std::array<Inputstate, GLFW_KEY_LAST> m_Keys;

	// Array of mouse inputs
	std::array<Inputstate, GLFW_MOUSE_BUTTON_LAST> m_Buttons;

	// Offset of scroll wheel
	double m_YAxis = 0.0;
};