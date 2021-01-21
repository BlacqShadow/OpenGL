#include "Camera.h"
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
extern float deltaTime;

float Camera::m_Xoffset;
float Camera::m_Yoffset;
float Camera::m_Yaw;
float Camera::m_Pitch;
float Camera::m_Sensitivity;
bool Camera::s_FirstMouse;
float Camera::m_Distance;
glm::vec2 Camera::m_InitialMousePosition;
glm::mat4 Camera::m_View;

glm::vec3 Camera::m_FocalPoint;
GLFWwindow* Camera::m_Window;

glm::vec3 Camera::m_CameraPosition;
glm::vec3 Camera::m_CameraFront;
glm::vec3 Camera::m_CameraUp;
float Camera::m_CameraSpeed;

constexpr auto M_PI = 3.14159f;;

// When initializing the window context for callback functions
Camera::Camera(GLFWwindow* window)
{
	m_InitialMousePosition = { 1280 / 2, 720 / 2 };
	m_Window = window;
	//m_Yaw = 3 * ((float)M_PI / 2);
	//m_Pitch = (float)M_PI / 2;
	m_Yaw = 20.0f * (M_PI / 180);
	m_Pitch = 10.0f * (M_PI / 180);

	glm::vec3 distance = { 5, 5, 5 };
	m_FocalPoint = { 0, 0, 0 };

	m_Distance = glm::distance(m_FocalPoint, distance);

	// Use this callback for single key events
	glfwSetKeyCallback(m_Window, Camera::KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
}


Camera::~Camera()
{
}

void Camera::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Exit out of the viewport when esc is pressed
	/*if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetCursorPosCallback(window, NULL);
	}*/
	
}


void Camera::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	// Check to see if the program is running for the very first time
	//if (s_FirstMouse)
	//{
	//	m_LastX = xpos;
	//	m_LastY = ypos;
	//	s_FirstMouse = false;
	//}
	//m_Xoffset = xpos - m_LastX;
	//m_Yoffset = m_LastY - ypos; // As y co-ordinates range from bottom to top
	//
	//m_LastX = xpos;
	//m_LastY = ypos;

	//m_Xoffset *= m_Sensitivity;
	//m_Yoffset *= m_Sensitivity;

	//m_Yaw += m_Xoffset;
	//m_Pitch += m_Yoffset;

	//// Make sure that the pitch is not higer than 90 degrees as that flips everything and causes wierd artifacts
	//if (m_Pitch > 89.0f)
	//	m_Pitch = 89.0f;
	//if (m_Pitch < -89.0f)
	//	m_Pitch = -89.0f;

	//// Add the values to the camera front value
	//
	//glm::vec3 front;
	//front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	//front.y = sin(glm::radians(m_Pitch));
	//front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	//

	//m_CameraFront = glm::normalize(front);

	// glm::vec2 offset = { xpos - m_InitialMousePosition.x, ypos - m_InitialMousePosition.y };

	std::cout << "x: " << xpos << " y: " << ypos << std::endl;



}


void Camera::processInput()
{
	m_CameraSpeed = 7.0f * deltaTime;
	/*if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
		m_CameraPosition += m_CameraSpeed * m_CameraFront;
	if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
		m_CameraPosition -= m_CameraSpeed * m_CameraFront;
	if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
		m_CameraPosition -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * m_CameraSpeed;
	if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
		m_CameraPosition += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * m_CameraSpeed;*/

	//if (/*glfwGetKey(m_Window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS */ glfwGetKey(m_Window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
	//{
	//	std::cout << "Middle mouse pressed" << std::endl;
	//}
}

glm::mat4 Camera::GetViewMatrix()
{
	glm::quat orientation = GetOrientation();

	// So we need the translation 
	m_CameraPosition = GetPosition();
	m_View = glm::translate(glm::mat4(1.0f), m_CameraPosition) * glm::mat4(orientation);
	m_View = glm::inverse(m_View);
	return m_View;
}

void Camera::EnableMouseInput()
{
	// glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// glfwSetCursorPosCallback(m_Window, MouseCallback);
	
}


glm::vec3 Camera::GetForwardDirection()
{
	return GetOrientation() * glm::vec3(0.0f, 0.0f, -1.0f);
}

glm::vec3 Camera::GetPosition()
{
	return m_FocalPoint - GetForwardDirection() * m_Distance;
	// return m_FocalPoint - GetForwardDirection();
}

glm::quat Camera::GetOrientation()
{
	return glm::quat(glm::vec3(-m_Pitch, m_Yaw, 0.0f));
}

void Camera::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	switch (action)
	{
		case GLFW_PRESS:
			if (button == GLFW_MOUSE_BUTTON_MIDDLE && glfwGetKey(m_Window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
			{
				double xpos, ypos;
				glfwGetCursorPos(m_Window, &xpos, &ypos);
				glm::vec2 offset(m_InitialMousePosition.x - xpos, m_InitialMousePosition.y - ypos);
				Pan(offset);
			}
			break;
	}
}



void Camera::Pan(glm::vec2 offset)
{
	throw std::logic_error("The method or operation is not implemented.");
}
