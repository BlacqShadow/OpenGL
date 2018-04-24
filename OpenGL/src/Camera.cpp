#include "Camera.h"
#include <iostream>
extern float deltaTime;

float Camera::m_Xoffset;
float Camera::m_Yoffset;
float Camera::m_yaw;
float Camera::m_pitch;
float Camera::m_LastX;
float Camera::m_LastY;
float Camera::m_Sensitivity;
bool Camera::s_FirstMouse;

glm::vec3 Camera::m_CameraPosition;
glm::vec3 Camera::m_CameraFront;
glm::vec3 Camera::m_CameraUp;

// When initializing the window context for callback functions
Camera::Camera(GLFWwindow* window)
	:m_Window(window)
{
	m_LastX = 1280.0f / 2.0f;
	m_LastY = 720.0f / 2.0f;
	m_Sensitivity = 0.05f;
	s_FirstMouse = true;

	// Variables to control the camera
	m_CameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);		// Position of the camera
	m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);		// The direction vector we want the camera to be looking at
	m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	//Default pitch and yaw values
	// We want the camera centered in both the x and y axis
	// where x axis would be the pitch
	// and y axis would be the pitch, in openGL angles are calculated counterclockwise. 
	m_yaw = -90.0f;
	m_pitch = 0.0f;
	// Use this callback for single key events
	glfwSetKeyCallback(m_Window, Camera::KeyCallback);
}


Camera::~Camera()
{
}
void Camera::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Exit out of the viewport when esc is pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}


void Camera::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	std::cout << "MOUSE MOVING" << std::endl;
	// Check to see if the program is running for the very first time
	if (s_FirstMouse)
	{
		m_LastX = xpos;
		m_LastY = ypos;
		s_FirstMouse = false;
	}
	m_Xoffset = xpos - m_LastX;
	m_Yoffset = m_LastY - ypos; // As y co-ordinates range from bottom to top
	
	m_LastX = xpos;
	m_LastY = ypos;

	m_Xoffset *= m_Sensitivity;
	m_Yoffset *= m_Sensitivity;

	m_yaw += m_Xoffset;
	m_pitch += m_Yoffset;

	// Make sure that the pitch is not higer than 90 degrees as that flips everything and causes wierd artifacts
	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	// Add the values to the camera front value
	
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	

	m_CameraFront = glm::normalize(front);


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

void Camera::enableMouseInput()
{
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(m_Window, MouseCallback);

}

