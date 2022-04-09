#pragma once

struct camera
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Camera"
	};

	void RotateRight(const float Speed = 1.f)
	{
		m_Azimuth -= Speed;

		if (m_Azimuth < 0.f)
			m_Azimuth += 360.f;
	}

	void RotateLeft(const float Speed = 1.f)
	{
		m_Azimuth += Speed;

		if (m_Azimuth > 360.f)
			m_Azimuth -= 360.f;
	}

	void RotateUp(const float Speed = 1.f)
	{
		m_Theta += Speed;

		if ( m_Theta > m_MaxTheta )  // Cap - 179
			m_Theta = m_MaxTheta;
	}

	void RotateDown(const float Speed = 1.f)
	{
		m_Theta -= Speed;

		if ( m_Theta < m_MinTheta ) // Cap - 1
			m_Theta = m_MinTheta;
	}

	void UpdateView(const glm::vec3& Position)
	{
		m_Target = Position;

		float cosTheta = glm::cos(glm::radians(m_Theta));
		float sinTheta = glm::sin(glm::radians(m_Theta));
		float cosAzimuth = glm::cos(glm::radians(m_Azimuth));
		float sinAzimuth = glm::sin(glm::radians(m_Azimuth));

		glm::vec3 camPosition;
		camPosition.x = m_Radius * sinAzimuth * sinTheta;
		camPosition.y = m_Radius * cosTheta;
		camPosition.z = m_Radius * cosAzimuth * sinTheta;

		m_Position = m_Target + camPosition;

		m_Front = m_Target - m_Position;
		m_Right = glm::cross(m_Front, glm::vec3{ 0.f, 1.f, 0.f });
		m_Up = glm::cross(m_Right, m_Front);

		m_Front = glm::normalize(m_Front);
		m_Right = glm::normalize(m_Right);
		m_Up = glm::normalize(m_Up);

		m_View = glm::lookAt(m_Position, m_Target, m_Up);
	}

	glm::mat4 m_View;
	glm::mat4 m_Projection = glm::perspective(glm::radians(59.f), 1920.f / 1080.f, 0.7f, 200.f);
	glm::vec3 m_Target = glm::vec3{ 0.f, 0.f, 0.f };
	glm::vec3 m_Position;

	glm::vec3 m_Right = glm::vec3{ 1.f, 0.f, 0.f };
	glm::vec3 m_Up = glm::vec3{ 0.f, 1.f, 0.f };
	glm::vec3 m_Front;

	paperback::Vector3f m_Offset = { 0.f, 0.f, 0.f };
	float m_Radius = 1.f;
	float m_MaxRadius = 2.f;
	float m_MinRadius = 2.f;
	float m_Theta = 90.f;
	float m_Azimuth = 0.f;
	float m_ZoomRate  = 1.f;
	float m_MinTheta = 55.f;
	float m_MaxTheta = 95.f;
	
	bool m_Active = true;
};

namespace RR_Camera
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<camera>(camera::typedef_v.m_pName)
			.constructor()(rttr::policy::ctor::as_object)
			.property("Active", &camera::m_Active)(rttr::policy::prop::as_reference_wrapper)
			.property("Camera Offset", &camera::m_Offset)(rttr::policy::prop::as_reference_wrapper)
			.property("Radius", &camera::m_Radius)(rttr::policy::prop::as_reference_wrapper)
			.property("Max Radius", &camera::m_MaxRadius)(rttr::policy::prop::as_reference_wrapper)
			.property("Min Radius", &camera::m_MinRadius)(rttr::policy::prop::as_reference_wrapper)
			.property("Zoom Rate", &camera::m_ZoomRate)(rttr::policy::prop::as_reference_wrapper)
			.property("Theta", &camera::m_Theta)(rttr::policy::prop::as_reference_wrapper)
			.property("Azimuth", &camera::m_Azimuth)(rttr::policy::prop::as_reference_wrapper)
			.property("Min Theta", &camera::m_MinTheta)(rttr::policy::prop::as_reference_wrapper)
			.property("Max Theta", &camera::m_MaxTheta)(rttr::policy::prop::as_reference_wrapper);
	}
}