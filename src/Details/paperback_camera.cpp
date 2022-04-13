/**********************************************************************************
*\file         paperback_camera_inline.h
*\brief        paperback_camera_inline.h
*
*\author	   Malcolm, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "../build/Paperback_V2/paperback_pch.h"
#include "../paperback_camera.h"
#include "glm/inc/gtx/transform.hpp"

cam::cam() :
	m_Right{ glm::vec3{1.f, 0.f, 0.f} },
	m_Up{ glm::vec3{0.f, 1.f, 0.f} },
	m_Radius{ 1.f },
	m_Azimuth{ 0.f },
	m_Theta{ 75.f }
{
	m_Target = glm::vec3{ 0.f, 3.f, -5.f };
	m_Position = glm::vec3{ 0.f, 15.f, 30.f };

	m_Front = m_Target - m_Position;
	UpdateVectors();
	m_View = glm::lookAt(m_Position, m_Target, m_Up);
	m_Projection = glm::perspective(glm::radians(59.f), 1920.f / 1080.f, 1.f, 200.f);
}

glm::vec3 cam::GetForwardVector() const
{
	return m_Front;
}

glm::mat4 cam::GetView() const
{
	return m_View;
}

glm::mat4 cam::GetProjection() const
{
	return m_Projection;
}

glm::vec3 cam::GetPosition() const
{
	return m_Position;
}

void cam::SetPosition(const glm::vec3& Position)
{
	glm::vec3 lookDirection = glm::normalize(m_Target - m_Position);

	m_Position = Position;
	m_Target = lookDirection + m_Position;

	UpdateView();
}

void cam::SetTarget(const glm::vec3& Target)
{
	glm::vec3 lookDirection = glm::normalize(Target - m_Position);
	m_Target = lookDirection + m_Position;

	m_Front = m_Target - m_Position;
	m_Right = glm::cross(m_Front, glm::vec3{ 0.f, 1.f, 0.f });
	m_Up = glm::cross(m_Right, m_Front);

	m_Front = glm::normalize(m_Front);
	m_Right = glm::normalize(m_Right);
	m_Up = glm::normalize(m_Up);

	UpdateView();

	glm::vec3 offsetDirection = m_Position - m_Target;
	m_Theta = glm::degrees(glm::acos(offsetDirection.y));
	m_Azimuth = glm::degrees(glm::atan(offsetDirection.x, offsetDirection.z));
}

void cam::MoveForward()
{
	glm::vec3 translate = m_Front * 0.1f;
	m_Target += translate;
	m_Position += translate;

	UpdateView();
}

void cam::MoveBackward()
{
	glm::vec3 translate = m_Front * 0.1f;
	m_Target -= translate;
	m_Position -= translate;

	UpdateView();
}

void cam::MoveRight()
{
	glm::vec3 translate = m_Right * 0.1f;
	m_Target += translate;
	m_Position += translate;

	UpdateView();
}

void cam::MoveLeft()
{
	glm::vec3 translate = m_Right * 0.1f;
	m_Target -= translate;
	m_Position -= translate;

	UpdateView();
}

void cam::MoveUp()
{
	glm::vec3 translate = m_Up * 0.1f;
	m_Target += translate;
	m_Position += translate;

	UpdateView();
}

void cam::MoveDown()
{
	glm::vec3 translate = m_Up * 0.1f;
	m_Target -= translate;
	m_Position -= translate;

	UpdateView();
}

void cam::RotateRight(const float m_Speed)
{
	m_Azimuth -= m_Speed;

	if (m_Azimuth < 0.f)
		m_Azimuth += 360.f;

	UpdateVectors();

	UpdateView();
}

void cam::RotateLeft(const float m_Speed)
{
	m_Azimuth += m_Speed;

	if (m_Azimuth > 360.f)
		m_Azimuth -= 360.f;

	UpdateVectors();

	UpdateView();
}

void cam::RotateUp(const float m_Speed)
{
	m_Theta += m_Speed;

	if (m_Theta > 179.f)
		m_Theta = 179.f;

	UpdateVectors();

	UpdateView();
}

void cam::RotateDown(const float m_Speed)
{
	m_Theta -= m_Speed;

	if (m_Theta < 1.f)
		m_Theta = 1.f;

	UpdateVectors();

	UpdateView();
}


void cam::RotateWithMouse(glm::vec2 Direction)
{
	glm::vec2 direction = Direction;
	float length = glm::length(direction);

	length = length <= 50.f ? length : 50.f;

	direction = glm::normalize(direction) * length * 0.01f;

	if (direction.x < 0)
	{
		RotateLeft(direction.x * -1.f);
	}
	else if (direction.x > 0)
	{
		RotateRight(direction.x);
	}

	if (direction.y > 0)
	{
		RotateDown(direction.y);
	}
	else if (direction.y < 0)
	{
		RotateUp(direction.y * -1.f);
	}
}

void cam::UpdateView()
{
	m_View = glm::lookAt(m_Position, m_Target, m_Up);
}

void cam::UpdateVectors()
{
	float cosTheta = glm::cos(glm::radians(m_Theta));
	float sinTheta = glm::sin(glm::radians(m_Theta));
	float cosAzimuth = glm::cos(glm::radians(m_Azimuth));
	float sinAzimuth = glm::sin(glm::radians(m_Azimuth));

	// Direction from point to position
	glm::vec3 offsetDirection;
	offsetDirection.x = m_Radius * sinAzimuth * sinTheta;
	offsetDirection.y = m_Radius * cosTheta;
	offsetDirection.z = m_Radius * cosAzimuth * sinTheta;

	m_Target = m_Position - offsetDirection;

	m_Front = m_Target - m_Position;
	m_Right = glm::cross(m_Front, glm::vec3{ 0.f, 1.f, 0.f });
	m_Up = glm::cross(m_Right, m_Front);

	m_Front = glm::normalize(m_Front);
	m_Right = glm::normalize(m_Right);
	m_Up = glm::normalize(m_Up);
}

cam& cam::GetInstanced()
{
	static cam camera;

	return camera;
}