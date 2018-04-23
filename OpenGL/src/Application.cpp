#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Camera.h"

// GLM Math Library
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"


// Keep track of time between frames
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Define instances
glm::vec3 Camera::m_CameraPosition;
glm::vec3 Camera::m_CameraFront;
glm::vec3 Camera::m_CameraUp;
float Camera::m_CameraSpeed = deltaTime * 2.5f;




int main(void)
{
	// Keep track of deltatime
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Switch to OpenGL core profile */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1280, 720, "OpenGL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	// Limit the framerate to the refresh rate of the monitor
	glfwSwapInterval(1);

	std::cout << glGetString(GL_VERSION) << std::endl;
	// Add a scope so that the objects ib and vb objects are destroyed before the openGL context
	{
		// First two are Vertex positions,
		// Next Two(same lines) - texture co-ordinates. (0.0 start, 1.0 finish)(Starts bottom left)
		float positions[] = {
			// Positions        Tex-Coord
			-0.5f,  0.5f, 0.0f,	0.0f, 1.0f,// 0
			 0.5f,  0.5f, 0.0f,	1.0f, 1.0f,// 1
			 0.5f, -0.5f, 0.0f,	1.0f, 0.0f,// 2
			-0.5f, -0.5f, 0.0f,	0.0f, 0.0f // 3
		};

		/* To prevent vertex duplication, using indices to refer to the vertex we need to draw a square. */
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};
		
		/* BLENDING */
		// Enable blending
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// Create a Vertex Array Object
		VertexArray va;
		// Create a Vertex buffer
		VertexBuffer vb(positions, 5 * 4 * sizeof(float));
		// Create the layout of the buffer
		VertexBufferLayout layout;
		// Tell the layout that each vertex is made out of two floats
		layout.Push<float>(3);
		layout.Push<float>(2);	// For the texture co-ordinates
		// Add the buffer to the Vertex Array object
		// Add the layout to the VAO
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		// Using a perspective matrix instead
		glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

		// Creating a model matrix
		glm::mat4 model;
		
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
		// Pass in a uniform with the slot the texture is bound to. 
		shader.SetUniform1i("u_Texture", 0);
		Texture texture("res/textures/ChernoLogo.png");

		/* Unbind everything for vertex array demostration */
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();
		texture.Bind();
		// Bind the texture

		Renderer renderer;

		// IMGUI CODE
		// Setup ImGui binding
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		//Variables for pitch and yaw
		float pitch;
		float yaw;


		/************************************************************************/
		Camera camera(window);
		// Variables to control the camera
		Camera::m_CameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);		// Position of the camera
		Camera::m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);		// The direction vector we want the camera to be looking at
		Camera::m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);		
		/************************************************************************/



		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			// IMGUI Frame can be put anywhere you want aslong as the imgui frame is in between them
			ImGui_ImplGlfwGL3_NewFrame();

			// Start polling for continous input
			camera.processInput();

			/* Bind all the buffers again before issuing a draw call */
			shader.Bind();
			// set the colors on the fly

			//Keep the matrices updated in real time
			glm::mat4 mvp = proj * camera.GetViewMatrix() * model;
			// SET the projection matrix in the shader
			shader.SetUniformMat4f("u_MVP", mvp);
			// Use the renderer to draw stuff 
			renderer.Draw(va, ib, shader);
			
			// IMGUI STUFF
			{
				ImGui::SliderFloat("X-Axis", &Camera::m_CameraPosition.x, -10.0f, 10.0f);
				ImGui::SliderFloat("Y-Axis", &Camera::m_CameraPosition.y, -100.0f, 100.0f);
				ImGui::SliderFloat("Z-Axis", &Camera::m_CameraPosition.z, -10.0f, 12.0f);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}