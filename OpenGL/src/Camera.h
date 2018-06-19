#pragma once
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <GLFW/glfw3.h>

class Camera
{
public:
	// Camera variables
	// Variables to control the camera
	static glm::vec3 m_CameraPosition;	// Position of the camera
	static glm::vec3 m_CameraFront;	// The direction vector we want the camera to be looking at
	static glm::vec3 m_CameraUp;
	GLFWwindow* m_Window;
	static float m_CameraSpeed;
	static float m_Xoffset;
	static float m_Yoffset;
	static float m_yaw;
	static float m_pitch;

private:
	static float m_LastX; 
	static float m_LastY;
	static float m_Sensitivity;
	static bool s_FirstMouse;

	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
private:
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
public:
	Camera(GLFWwindow* window);
	~Camera();
	void processInput();
	static glm::mat4 GetViewMatrix();
	void enableMouseInput();
};

