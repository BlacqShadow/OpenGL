#include "Camera.h"
#include <iostream>
extern float deltaTime;
void Camera::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	m_CameraSpeed = 2.5f * deltaTime;
	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
		std::cout << "W PRESSED" << std::endl;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_CameraPosition += m_CameraSpeed * m_CameraFront;
	else if (key == GLFW_KEY_S && action == GLFW_PRESS)
		m_CameraPosition -= m_CameraSpeed * m_CameraFront;
	else if (key == GLFW_KEY_A && action == GLFW_PRESS)
		m_CameraPosition -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * m_CameraSpeed;
	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
		m_CameraPosition += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * m_CameraSpeed;
}


// When initializing the window context for callback functions
Camera::Camera(GLFWwindow* window)
	:m_Window(window)
{
	// Use this for single key events
	//glfwSetKeyCallback(m_Window, Camera::KeyCallback);
}


Camera::~Camera()
{
}

void Camera::processInput()
{
	m_CameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
		std::cout << "W PRESSED" << std::endl;
	if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
		m_CameraPosition += m_CameraSpeed * m_CameraFront;
	if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
		m_CameraPosition -= m_CameraSpeed * m_CameraFront;
	if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
		m_CameraPosition -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * m_CameraSpeed;
	if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
		m_CameraPosition += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * m_CameraSpeed;
}

glm::mat4 Camera::GetViewMatrix()
{
	glm::mat4 view = glm::lookAt(
		m_CameraPosition,					// position of the camera
		m_CameraPosition + m_CameraFront,	// The point the camera will be looking at 
		m_CameraUp						// The up axis, used to calculate the correct co-ordinate axis for the view matrix
	);
	return view;
}


