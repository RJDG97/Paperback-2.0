#pragma once

#include <iostream>
#include "Mono.h"
#include "paperback_camera.h"
#include "paperback_input.h"

struct scripting_system : paperback::system::instance
{
	Mono* m_pMono = nullptr;

	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "scripting_system"
	};

	void OnSystemCreated(void) noexcept
	{
		// Set up Mono
		m_pMono = new Mono;
	}

	void Update(void) noexcept 
	{
		//m_pMono->externaltest();

		if (m_Coordinator.m_Input.IsKeyPressDown(GLFW_KEY_W))
		{
			Camera::GetInstanced().MoveForward();
		}

		if (m_Coordinator.m_Input.IsKeyPressDown(GLFW_KEY_S))
		{
			Camera::GetInstanced().MoveBackward();
		}

		if (m_Coordinator.m_Input.IsKeyPressDown(GLFW_KEY_A))
		{
			Camera::GetInstanced().MoveLeft();
		}

		if (m_Coordinator.m_Input.IsKeyPressDown(GLFW_KEY_D))
		{
			Camera::GetInstanced().MoveRight();
		}

		if (m_Coordinator.m_Input.IsKeyPressDown(GLFW_KEY_Q))
		{
			Camera::GetInstanced().MoveUp();
		}

		if (m_Coordinator.m_Input.IsKeyPressDown(GLFW_KEY_E))
		{
			Camera::GetInstanced().MoveDown();
		}

		if (m_Coordinator.m_Input.IsKeyPressDown(GLFW_KEY_UP))
		{
			Camera::GetInstanced().RotateUp();
		}

		if (m_Coordinator.m_Input.IsKeyPressDown(GLFW_KEY_DOWN))
		{
			Camera::GetInstanced().RotateDown();
		}

		if (m_Coordinator.m_Input.IsKeyPressDown(GLFW_KEY_LEFT))
		{
			Camera::GetInstanced().RotateLeft();
		}

		if (m_Coordinator.m_Input.IsKeyPressDown(GLFW_KEY_RIGHT))
		{
			Camera::GetInstanced().RotateRight();
		}
	}

	void OnSystemTerminated(void) noexcept 
	{
		// you might have to delete m_pMono too
		delete m_pMono;
	}
};