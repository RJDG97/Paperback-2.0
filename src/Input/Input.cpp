//#include "../Application.h"
//#include "../Source/Camera/Camera.h"
#include "Input.h"

Input::Input()
{
	m_Keys.fill({ GLFW_RELEASE, GLFW_RELEASE });
	m_Buttons.fill({ -1,-1 });
}

void Input::SetKey(int Key, int Action)
{
	// Set key current state
	m_Keys[Key].m_Current = Action;
}

bool Input::IsKeyPress(int Key) const
{
	// Get key reference
	const auto& KeyCode = m_Keys[Key];

	// Check state
	return KeyCode.m_Current == GLFW_PRESS && KeyCode.m_Previous != GLFW_PRESS;
}

bool Input::IsKeyPressDown(int Key) const
{
	// Get key reference
	const auto& KeyCode = m_Keys[Key];

	// Check state
	return KeyCode.m_Current == GLFW_REPEAT || (KeyCode.m_Current == GLFW_PRESS && KeyCode.m_Previous == GLFW_PRESS);
}

bool Input::IsKeyPressUp(int Key) const
{
	// Get key reference
	const auto& KeyCode = m_Keys[Key];

	// Check state
	return KeyCode.m_Current == GLFW_RELEASE && KeyCode.m_Previous != KeyCode.m_Current;
}

void Input::SetMouse(int Key, int Action)
{
	// Set button state
	m_Buttons[Key].m_Current = Action;
}

void Input::SetScroll(double Y)
{
	m_YAxis = Y;
}

bool Input::IsMousePress(int Key)
{
	// Get button reference
	const auto& ButtonCode = m_Buttons[Key];

	// Check button state
	return ButtonCode.m_Current == GLFW_PRESS && ButtonCode.m_Current != ButtonCode.m_Previous;
}

bool Input::IsMouseDown(int Key) const
{
	// Check button state
	return m_Buttons[Key].m_Current == GLFW_PRESS;
}

bool Input::IsMouseUp(int Key)
{
	// Get button reference
	const auto& ButtonCode = m_Buttons[Key];

	// Check button state
	return ButtonCode.m_Current == GLFW_RELEASE && ButtonCode.m_Current != ButtonCode.m_Previous;
}

bool Input::IsScrollUp()
{
	// Check displacement
	if (m_YAxis < 0.0)
	{
		m_YAxis = 0.0;
		return true;
	}

	return false;
}

bool Input::IsScrollDown()
{
	// Check displacement
	if (m_YAxis > 0.0)
	{
		m_YAxis = 0.0;
		return true;
	}

	return false;
}

glm::vec3 Input::GetMousePosition() const
{
	//// Get x and y values
	//double X, Y;
	//glfwGetCursorPos(Application::GetInstance().GetWindow(), &X, &Y);

	//// Find point in NDC
	//glm::vec4 NDCPoint = glm::vec4{ (2.f * X) / Application::GetInstance().GetWidth() - 1.f, 1.f - (2.f * Y) / Application::GetInstance().GetHeight(), -1.f, 1.f };
	//// Find point in view
	//glm::vec4 ViewPoint = glm::inverse(Camera::GetInstanced().GetProjection()) * NDCPoint;
	//ViewPoint = glm::vec4{ ViewPoint.x, ViewPoint.y, -1.f, 0.f };
	//// Find point world
	//glm::vec3 WorldPoint = glm::inverse(Camera::GetInstanced().GetView()) * ViewPoint;

	//return WorldPoint;

	return glm::vec3{ 0 };
}

void Input::UpateInputs()
{
	for (auto& key : m_Keys)
		key.m_Previous = key.m_Current;

	for (auto& mouse : m_Buttons)
		mouse.m_Previous = mouse.m_Current;
}

Input& Input::GetInstance()
{
	static Input input;

	return input;
}