#ifndef CAMERA_H
#define CAMERA_H

#include "glm/inc/glm.hpp"

class Camera
{
public:
	~Camera() = default;

	glm::mat4 GetView() const;
	glm::mat4 GetProjection() const;
	glm::vec3 GetPosition() const;

	void MoveForward();
	void MoveBackward();
	void MoveRight();
	void MoveLeft();
	void MoveUp();
	void MoveDown();

	void RotateRight();
	void RotateLeft();
	void RotateUp();
	void RotateDown();

	static Camera& GetInstanced();
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

private:
	Camera();

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
