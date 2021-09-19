#include "../build/Paperback_V2/paperback_pch.h"
#include "../paperback_camera.h"
#include "glm/inc/gtx/transform.hpp"


Camera::Camera() :
	m_Target{ glm::vec3{ 0.f } },
	m_Position{ glm::vec3{ 0.f, 0.f, 1.f } },
	m_Right{ glm::vec3{1.f, 0.f, 0.f} },
	m_Up{ glm::vec3{0.f, 1.f, 0.f} },
	m_Radius{ 1.f },
	m_Azimuth{ 0.f },
	m_Theta{ 90.f }
{
	m_Front = m_Target - m_Position;
	m_View = glm::lookAt(m_Position, m_Target, m_Up);
	m_Projection = glm::perspective(glm::radians(59.f), 1920.f / 1080.f, 1.f, 200.f);
	//m_Projection = glm::ortho(-960.f, 960.f, -540.f, 540.f, 1.f, 200.f);
}

glm::mat4 Camera::GetView() const
{
	return m_View;
}

glm::mat4 Camera::GetProjection() const
{
	return m_Projection;
}

void Camera::MoveForward()
{
	glm::vec3 translate = m_Front * 0.1f;
	m_Target += translate;
	m_Position += translate;

	UpdateView();
}

void Camera::MoveBackward()
{
	glm::vec3 translate = m_Front * 0.1f;
	m_Target -= translate;
	m_Position -= translate;

	UpdateView();
}

void Camera::MoveRight()
{
	glm::vec3 translate = m_Right * 0.1f;
	m_Target += translate;
	m_Position += translate;

	UpdateView();
}

void Camera::MoveLeft()
{
	glm::vec3 translate = m_Right * 0.1f;
	m_Target -= translate;
	m_Position -= translate;

	UpdateView();
}

void Camera::MoveUp()
{
	glm::vec3 translate = m_Up * 0.1f;
	m_Target += translate;
	m_Position += translate;

	UpdateView();
}

void Camera::MoveDown()
{
	glm::vec3 translate = m_Up * 0.1f;
	m_Target -= translate;
	m_Position -= translate;

	UpdateView();
}

void Camera::RotateRight()
{
	m_Azimuth -= 0.1f;

	if (m_Azimuth < 0.f )
		m_Azimuth = 360.f;

	UpdateVectors();

	UpdateView();
}

void Camera::RotateLeft()
{
	m_Azimuth += 0.1f;

	if (m_Azimuth > 360.f)
		m_Azimuth = 0.f;

	UpdateVectors();

	UpdateView();
}

void Camera::RotateUp()
{
	m_Theta += 0.1f;

	if (m_Theta > 179.f)
		m_Theta = 179.f;

	UpdateVectors();

	UpdateView();
}

void Camera::RotateDown()
{
	m_Theta -= 0.1f;

	if (m_Theta < 1.f)
		m_Theta = 1.f;

	UpdateVectors();

	UpdateView();
}

void Camera::UpdateView()
{
	m_View = glm::lookAt(m_Position, m_Target, m_Up);
}

void Camera::UpdateVectors()
{
	float cosTheta = glm::cos(glm::radians(m_Theta));
	float sinTheta = glm::sin(glm::radians(m_Theta));
	float cosAzimuth = glm::cos(glm::radians(m_Azimuth));
	float sinAzimuth = glm::sin(glm::radians(m_Azimuth));

	glm::vec3 offset;
	offset.x = m_Radius * sinAzimuth * sinTheta;
	offset.y = m_Radius * cosTheta;
	offset.z = m_Radius * cosAzimuth * sinTheta;

	m_Target = m_Position - offset;

	m_Front = m_Target - m_Position;
	m_Right = glm::cross(m_Front, glm::vec3{ 0.f, 1.f, 0.f });
	m_Up = glm::cross(m_Right, m_Front);

	m_Front = glm::normalize(m_Front);
	m_Right = glm::normalize(m_Right);
	m_Up = glm::normalize(m_Up);
}

Camera& Camera::GetInstanced()
{
	static Camera camera;

	return camera;
}