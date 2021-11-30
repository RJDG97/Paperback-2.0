#ifndef CAMERA_H
#define CAMERA_H

#include "glm/inc/glm.hpp"

class Camera3D
{
public:
	~Camera3D() = default;

	glm::vec3 GetForwardVector() const;

	glm::mat4 GetView() const;
	glm::mat4 GetProjection() const;
	glm::vec3 GetPosition() const;

	void SetPosition(const glm::vec3& Position);
	void SetTarget(const glm::vec3& Target);

	void MoveForward();
	void MoveBackward();
	void MoveRight();
	void MoveLeft();
	void MoveUp();
	void MoveDown();

	void RotateRight(const float m_Speed = 0.1f);
	void RotateLeft(const float m_Speed = 0.1f);
	void RotateUp(const float m_Speed = 0.1f);
	void RotateDown(const float m_Speed = 0.1f);

	void RotateWithMouse();

	static Camera3D& GetInstanced();
	Camera3D(const Camera3D&) = delete;
	Camera3D& operator=(const Camera3D&) = delete;

private:
	Camera3D();
	
	void UpdateView();
	void UpdateVectors();

	glm::mat4 m_View;
	glm::mat4 m_Projection;
	glm::vec3 m_Target;
	glm::vec3 m_Position;

	glm::vec3 m_Right;
	glm::vec3 m_Up;
	glm::vec3 m_Front;

	float m_Radius;
	float m_Theta;
	float m_Azimuth;
};

#endif
