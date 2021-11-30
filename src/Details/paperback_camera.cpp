#include "../build/Paperback_V2/paperback_pch.h"
#include "../paperback_camera.h"
#include "glm/inc/gtx/transform.hpp"

Camera3D::Camera3D() :
	m_Right{ glm::vec3{1.f, 0.f, 0.f} },
	m_Up{ glm::vec3{0.f, 1.f, 0.f} },
	m_Radius{ 1.f },
	m_Azimuth{ 0.f },
	m_Theta{ 60.f }
{
	m_Target = glm::vec3{ 0.f, 3.f, -5.f };
	m_Position = glm::vec3{ 0.f, 3.f, 0.f };

	m_Front = m_Target - m_Position;
	UpdateVectors();
	m_View = glm::lookAt(m_Position, m_Target, m_Up);
	m_Projection = glm::perspective(glm::radians(59.f), 1920.f / 1080.f, 1.f, 200.f);
}

glm::vec3 Camera3D::GetForwardVector() const
{
	return m_Front;
}

glm::mat4 Camera3D::GetView() const
{
	return m_View;
}

glm::mat4 Camera3D::GetProjection() const
{
	return m_Projection;
}

glm::vec3 Camera3D::GetPosition() const
{
	return m_Position;
}

void Camera3D::SetPosition(const glm::vec3& Position)
{
	glm::vec3 lookDirection = glm::normalize(m_Target - m_Position);

	m_Position = Position;
	m_Target = lookDirection + m_Position;

	UpdateView();
}

void Camera3D::SetTarget(const glm::vec3& Target)
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

void Camera3D::MoveForward()
{
	glm::vec3 translate = m_Front * 0.1f;
	m_Target += translate;
	m_Position += translate;

	UpdateView();
}

void Camera3D::MoveBackward()
{
	glm::vec3 translate = m_Front * 0.1f;
	m_Target -= translate;
	m_Position -= translate;

	UpdateView();
}

void Camera3D::MoveRight()
{
	glm::vec3 translate = m_Right * 0.1f;
	m_Target += translate;
	m_Position += translate;

	UpdateView();
}

void Camera3D::MoveLeft()
{
	glm::vec3 translate = m_Right * 0.1f;
	m_Target -= translate;
	m_Position -= translate;

	UpdateView();
}

void Camera3D::MoveUp()
{
	glm::vec3 translate = m_Up * 0.1f;
	m_Target += translate;
	m_Position += translate;

	UpdateView();
}

void Camera3D::MoveDown()
{
	glm::vec3 translate = m_Up * 0.1f;
	m_Target -= translate;
	m_Position -= translate;

	UpdateView();
}

void Camera3D::RotateRight(const float m_Speed)
{
	m_Azimuth -= m_Speed;

	if (m_Azimuth < 0.f)
		m_Azimuth += 360.f;

	UpdateVectors();

	UpdateView();
}

void Camera3D::RotateLeft(const float m_Speed)
{
	m_Azimuth += m_Speed;

	if (m_Azimuth > 360.f)
		m_Azimuth -= 360.f;

	UpdateVectors();

	UpdateView();
}

void Camera3D::RotateUp(const float m_Speed)
{
	m_Theta += m_Speed;

	if (m_Theta > 179.f)
		m_Theta = 179.f;

	UpdateVectors();

	UpdateView();
}

void Camera3D::RotateDown(const float m_Speed)
{
	m_Theta -= m_Speed;

	if (m_Theta < 1.f)
		m_Theta = 1.f;

	UpdateVectors();

	UpdateView();
}


void Camera3D::RotateWithMouse()
{
	/*if (PPB.IsMouseDown(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glm::vec2 direction = PPB.GetMouseDirection();
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
	}*/
}

void Camera3D::UpdateView()
{
	m_View = glm::lookAt(m_Position, m_Target, m_Up);
}

void Camera3D::UpdateVectors()
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

Camera3D& Camera3D::GetInstanced()
{
	static Camera3D camera;

	return camera;
}