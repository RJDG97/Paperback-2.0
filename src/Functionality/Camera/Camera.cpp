#include "Camera.h"
#include <glm/inc/gtx/transform.hpp>

glm::mat4 Camera::GetView() const
{
	return m_View;
}

glm::mat4 Camera::GetProjection() const
{
	return m_Projection;
}

glm::vec3 Camera::GetPosition() const
{
	return m_Position;
}

Camera2D::Camera2D()
{
	m_Target = glm::vec3{ 0.f };
	m_Position = glm::vec3{ 0, 0, 10.f };
	m_Projection = glm::ortho(-960.f, 960.f, -540.f, 540.f, 1.f, 200.f);
	m_View = glm::lookAt(m_Position, m_Target, glm::vec3{ 0.f, 1.f, 0.f });
}

Camera3D::Camera3D() :
	m_Right{ glm::vec3{1.f, 0.f, 0.f} },
	m_Up{ glm::vec3{0.f, 1.f, 0.f} },
	m_Radius{ 1.f },
	m_Azimuth{ 0.f },
	m_Theta{ 90.f }

{
	m_Target = glm::vec3{ 0.f };
	m_Position = glm::vec3{ 0.f, 0.f, 1.f };

	m_Front = m_Target - m_Position;
	m_View = glm::lookAt(m_Position, m_Target, m_Up);
	m_Projection = glm::perspective(glm::radians(59.f), 1920.f / 1080.f, 1.f, 1000.f);
}

Camera3D::Camera3D(const Camera3D& Camera)
{
	m_Radius = Camera.m_Radius;
	m_Azimuth = Camera.m_Azimuth;
	m_Theta = Camera.m_Theta;

	m_Front = Camera.m_Front;
	m_Right = Camera.m_Right;
	m_Up = Camera.m_Up;

	m_Position = Camera.m_Position;
	m_Target = Camera.m_Target;

	m_View = Camera.m_View;
	m_Projection = Camera.m_Projection;
}

Camera3D& Camera3D::operator=(const Camera3D& Camera)
{
	m_Radius = Camera.m_Radius;
	m_Azimuth = Camera.m_Azimuth;
	m_Theta = Camera.m_Theta;

	m_Front = Camera.m_Front;
	m_Right = Camera.m_Right;
	m_Up = Camera.m_Up;

	m_Position = Camera.m_Position;
	m_Target = Camera.m_Target;

	m_View = Camera.m_View;
	m_Projection = Camera.m_Projection;

	return *this;
}

Camera3D::Camera3D(const glm::vec3& Position, const glm::mat4& View, const glm::mat4& Projection)
{
	m_Position = Position;

	m_View = View;
	m_Projection = Projection;
}

glm::vec3 Camera3D::GetForwardVector() const
{
	return m_Front;
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

void Camera3D::MoveForward(const float m_Speed)
{
	glm::vec3 translate = m_Front * m_Speed;
	m_Target += translate;
	m_Position += translate;

	UpdateView();
}

void Camera3D::MoveBackward(const float m_Speed)
{
	glm::vec3 translate = m_Front * m_Speed;
	m_Target -= translate;
	m_Position -= translate;

	UpdateView();
}

void Camera3D::MoveRight(const float m_Speed)
{
	glm::vec3 translate = m_Right * m_Speed;
	m_Target += translate;
	m_Position += translate;

	UpdateView();
}

void Camera3D::MoveLeft(const float m_Speed)
{
	glm::vec3 translate = m_Right * m_Speed;
	m_Target -= translate;
	m_Position -= translate;

	UpdateView();
}

void Camera3D::MoveUp(const float m_Speed)
{
	glm::vec3 translate = m_Up * m_Speed;
	m_Target += translate;
	m_Position += translate;

	UpdateView();
}

void Camera3D::MoveDown(const float m_Speed)
{
	glm::vec3 translate = m_Up * m_Speed;
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

void Camera3D::RotateWithMouse( glm::vec2 Direction, const float SensitivityRatio )
{
	// TO REPLACE W GLOBAL SENS
	auto MouseSensitivity = 200.0f;

	glm::vec2 direction = Direction;

	//float length = glm::length(direction);
	//length = length <= 50.f ? length : 50.f;

	direction = glm::normalize(direction) * MouseSensitivity * 0.01f;

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