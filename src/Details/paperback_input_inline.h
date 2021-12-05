#include "../paperback_camera.h"
#include "../../build/Sandbox/Systems/WindowSystem.h"

Input::Input( paperback::coordinator::instance& Coordinator) noexcept :
	m_Coordinator{ Coordinator }
{
	m_Coordinator.RegisterEvent<KeyPressed>();
	m_Coordinator.RegisterEvent<MousePressed>();

	m_YAxis = 0.0;
	m_Keys.fill({ GLFW_RELEASE, GLFW_RELEASE });
	m_Buttons.fill({ -1,-1 });
}


//-----------------------------------
//              Keys
//-----------------------------------

void Input::SetKey(int Key, int Action) noexcept
{
	// Set key current state
	m_Keys[Key].m_Current = static_cast<int8_t>(Action);
}

bool Input::IsKeyPress(int Key) const noexcept
{
	// Get key reference
	const auto& KeyCode = m_Keys[Key];

	// Check state
	return KeyCode.m_Current == GLFW_PRESS && KeyCode.m_Previous != GLFW_PRESS;
}

bool Input::IsKeyPressDown(int Key) const noexcept
{
	// Get key reference
	const auto& KeyCode = m_Keys[Key];

	// Check state
	return KeyCode.m_Current == GLFW_REPEAT || (KeyCode.m_Current == GLFW_PRESS && KeyCode.m_Previous == GLFW_PRESS);
}

bool Input::IsKeyPressUp(int Key) const noexcept
{
	// Get key reference
	const auto& KeyCode = m_Keys[Key];

	// Check state
	return KeyCode.m_Current == GLFW_RELEASE && KeyCode.m_Previous != KeyCode.m_Current;
}


//-----------------------------------
//              Mouse
//-----------------------------------

void Input::SetMouse(int Key, int Action) noexcept
{
	// Set button state
	m_Buttons[Key].m_Current = static_cast<int8_t>(Action);

	if (IsMousePress(GLFW_MOUSE_BUTTON_RIGHT))
	{
		double X, Y;
		GLFWwindow* m_pWindow = PPB.GetSystem< window_system >().m_pWindow;

		glfwGetCursorPos(m_pWindow, &X, &Y);

		m_MouseOriginPosition = glm::vec2{ X, Y };
	}
}

void Input::SetScroll(double Y) noexcept
{
	m_YAxis = Y;
}

bool Input::IsMousePress(int Key) const noexcept
{
	// Get button reference
	const auto& ButtonCode = m_Buttons[Key];

	// Check button state
	return ButtonCode.m_Current == GLFW_PRESS && ButtonCode.m_Current != ButtonCode.m_Previous;
}

bool Input::IsMouseDown(int Key) const noexcept
{
	// Check button state
	return m_Buttons[Key].m_Current == GLFW_PRESS;
}

bool Input::IsMouseUp(int Key) const noexcept
{
	// Get button reference
	const auto& ButtonCode = m_Buttons[Key];

	// Check button state
	return ButtonCode.m_Current == GLFW_RELEASE && ButtonCode.m_Current != ButtonCode.m_Previous;
}

bool Input::IsScrollUp() noexcept
{
	// Check displacement
	if (m_YAxis < 0.0)
	{
		m_YAxis = 0.0;
		return true;
	}

	return false;
}

bool Input::IsScrollDown() noexcept
{
	// Check displacement
	if (m_YAxis > 0.0)
	{
		m_YAxis = 0.0;
		return true;
	}

	return false;
}

glm::vec2 Input::GetMouseDirection() const noexcept
{
	double X, Y;
	GLFWwindow* m_pWindow = PPB.GetSystem< window_system >().m_pWindow;

	glfwGetCursorPos(m_pWindow, &X, &Y);

	return glm::vec2{ X, Y } - m_MouseOriginPosition;
}

glm::vec2 Input::Get2DMousePosition() const noexcept
{
	GLFWwindow* m_pWindow = PPB.GetSystem< window_system >().m_pWindow;
	auto& WindowDetails = PPB.GetSystem< window_system >().E;

	// Get x and y values
	double X, Y;
	glfwGetCursorPos(m_pWindow, &X, &Y);

	// Find point in NDC
	glm::vec2 NDCPoint = glm::vec2{ (2.f * X) / WindowDetails.m_Width - 1.f, 1.f - (2.f * Y) / WindowDetails.m_Height };

	return NDCPoint;
}

glm::vec3 Input::GetMousePosition() const noexcept
{
	GLFWwindow* m_pWindow = PPB.GetSystem< window_system >().m_pWindow;
	auto& WindowDetails = PPB.GetSystem< window_system >().E;

	// Get x and y values
	double X, Y;
	glfwGetCursorPos(m_pWindow, &X, &Y);

	// Find point in NDC
	glm::vec4 NDCPoint = glm::vec4{ (2.f * X) / WindowDetails.m_Width - 1.f, 1.f - (2.f * Y) / WindowDetails.m_Height, -1.f, 1.f };
	// Find point in view
	glm::vec4 ViewPoint = glm::inverse(Camera3D::GetInstanced().GetProjection()) * NDCPoint;
	ViewPoint = glm::vec4{ ViewPoint.x, ViewPoint.y, -1.f, 0.f };
	// Find point world
	glm::vec3 WorldPoint = glm::inverse(Camera3D::GetInstanced().GetView()) * ViewPoint;

	return WorldPoint;
}

glm::vec3 Input::GetViewportMousePosition(glm::vec2 viewport_min, glm::vec2 viewport_max) const noexcept
{
	GLFWwindow* m_pWindow = PPB.GetSystem< window_system >().m_pWindow;
	auto& WindowDetails = PPB.GetSystem< window_system >().E;

	// Get x and y values
	double X, Y;
	glfwGetCursorPos(m_pWindow, &X, &Y);

	if (X < viewport_min.x || X > viewport_max.x || Y < viewport_min.y || Y > viewport_max.y)
	{
		return {};
	}

	// Find point in NDC
	glm::vec4 NDCPoint = glm::vec4{ (2.f * (X- viewport_min.x)) / (viewport_max.x - viewport_min.x) - 1.f, 1.f - (2.f * (Y- viewport_min.y)) / (viewport_max.y - viewport_min.y), -1.f, 1.f };
	// Find point in view
	glm::vec4 ViewPoint = glm::inverse(Camera3D::GetInstanced().GetProjection()) * NDCPoint;
	ViewPoint = glm::vec4{ ViewPoint.x, ViewPoint.y, -1.f, 0.f };
	// Find point world
	glm::vec3 WorldPoint = glm::inverse(Camera3D::GetInstanced().GetView()) * ViewPoint;

	return WorldPoint;
}

//-----------------------------------
//           Post Update
//-----------------------------------

void Input::UpateInputs() noexcept
{
	//for (auto& key : m_Keys)
	for ( size_t i = 0, max = m_Keys.size(); i < max; ++i )
	{
		// Button Click Broadcast
		if ( m_Keys[i].m_Current == GLFW_PRESS && m_Keys[i].m_Previous != GLFW_PRESS )
			m_Coordinator.BroadcastEvent<KeyClicked>( i, true );
		// Button Pressed Broadcast - Not Releasing Yet
		else if ( m_Keys[i].m_Current == GLFW_PRESS && m_Keys[i].m_Previous == GLFW_PRESS )
			m_Coordinator.BroadcastEvent<KeyPressed>( i, false );

		m_Keys[i].m_Previous = m_Keys[i].m_Current;
	}

	for ( size_t i = 0, max = m_Buttons.size(); i < max; ++i )
	{
		// Mouse Click Broadcast
		if ( m_Buttons[i].m_Current != GLFW_PRESS && m_Buttons[i].m_Previous == GLFW_PRESS )
			m_Coordinator.BroadcastEvent<MouseClicked>( i, true );
		// Mouse Pressed Broadcast - Not Releasing Yet
		else if ( m_Buttons[i].m_Current == GLFW_PRESS && m_Buttons[i].m_Previous == GLFW_PRESS )
			m_Coordinator.BroadcastEvent<MousePressed>( i, false );

		m_Buttons[i].m_Previous = m_Buttons[i].m_Current;
	}
}