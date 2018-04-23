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

private:
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
	Camera(GLFWwindow* window);
	~Camera();
	void processInput();
	static glm::mat4 GetViewMatrix();
};

