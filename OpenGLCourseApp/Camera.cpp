#include "Camera.h"

Camera::Camera()
{

}

Camera::Camera(glm::vec3 initialPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = initialPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	forward = glm::vec3(0.0f, 0.0f, -1.0f);

	movementSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	Update();
}

void Camera::KeyControl(bool* keys, GLfloat deltaTime)
{
	if (keys[GLFW_KEY_W])
	{
		position += forward * movementSpeed * deltaTime;
	}
	if (keys[GLFW_KEY_S])
	{
		position -= forward * movementSpeed * deltaTime;
	}
	if (keys[GLFW_KEY_D])
	{
		position += right * movementSpeed * deltaTime;
	}
	if (keys[GLFW_KEY_A])
	{
		position -= right * movementSpeed * deltaTime;
	}
	if (keys[GLFW_KEY_E])
	{
		position += worldUp * movementSpeed * deltaTime;
	}
	if (keys[GLFW_KEY_Q])
	{
		position -= worldUp * movementSpeed * deltaTime;
	}
}

void Camera::MouseControl(GLfloat xChange, GLfloat yChange)
{
	yaw = yaw + xChange * turnSpeed;
	pitch = glm::clamp(pitch + yChange * turnSpeed, -89.0f, 89.0f);
	Update();
}

glm::mat4 Camera::CalculateViewMatrix()
{
	return glm::lookAt(position, position + forward, up);
}

glm::vec3 Camera::GetCameraPosition()
{
	return position;
}

glm::vec3 Camera::GetCameraDirection()
{
	return forward;
}

void Camera::Update()
{
	forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward.y = sin(glm::radians(pitch));
	forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward = glm::normalize(forward);

	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));
}

Camera::~Camera()
{

}