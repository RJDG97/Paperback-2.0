/**********************************************************************************
*\file         Camera.h
*\brief        Camera.h
*
*\author	   Malcolm, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include <glm/inc/glm.hpp>

class Camera
{
public:
	glm::mat4 GetView() const;
	glm::mat4 GetProjection() const;
	glm::vec3 GetPosition() const;

	glm::mat4 m_View;
	glm::mat4 m_Projection;
	glm::vec3 m_Target;
	glm::vec3 m_Position;
};

class Camera2D : public Camera
{
public:
	Camera2D();
	~Camera2D() = default;

	Camera2D(const Camera2D&) = delete;
	Camera2D& operator=(const Camera2D&) = delete;
};

class Camera3D : public Camera
{
public:
	Camera3D();
	~Camera3D() = default;

	Camera3D(const Camera3D&);
	Camera3D& operator=(const Camera3D&);

	Camera3D(const glm::vec3& Position, const glm::mat4& View, const glm::mat4& Projection);

	glm::vec3 GetForwardVector() const;

	void SetPosition(const glm::vec3& Position);
	void SetTarget(const glm::vec3& Target);

	void MoveForward(const float m_Speed = 0.1f);
	void MoveBackward(const float m_Speed = 0.1f);
	void MoveRight(const float m_Speed = 0.1f);
	void MoveLeft(const float m_Speed = 0.1f);
	void MoveUp(const float m_Speed = 0.1f);
	void MoveDown(const float m_Speed = 0.1f);

	void RotateRight(const float m_Speed = 0.1f);
	void RotateLeft(const float m_Speed = 0.1f);
	void RotateUp(const float m_Speed = 0.1f);
	void RotateDown(const float m_Speed = 0.1f);

	void RotateWithMouse( glm::vec2 Direction, const float SensitivityRatio = 1.0f );

private:
	void UpdateView();
	void UpdateVectors();

	glm::vec3 m_Right;
	glm::vec3 m_Up;
	glm::vec3 m_Front;

	float m_Radius;
	float m_Theta;
	float m_Azimuth;
};