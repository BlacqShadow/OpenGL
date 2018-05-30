#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
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

#include "Model.h"

// Keep track of time between frames
// Not Implemented Yet
float deltaTime = 0.0f;
float lastFrame = 0.0f;

const float screenWidth = 1280.0f;
const float screenHeight = 720.0f;
const char* windowName = "OpenGL";


int main(void)
{
	// Keep track of deltatime
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Switch to OpenGL core profile */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow((int)screenWidth,(int)screenHeight, windowName, NULL, NULL);
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
		// Enable Depth buffer testing
		GLCall(glEnable(GL_DEPTH_TEST));

		// Mesh load test
		Model test("res/models/JackOOBJ.obj");
		const std::vector<Mesh*> meshes = test.GetMeshes();

		// Using a perspective matrix with a fov of 75
		glm::mat4 proj = glm::perspective(glm::radians(75.0f), screenWidth / screenHeight, 0.1f, 100.0f);

		// Creating a model matrix
		glm::mat4 model;
		
		// Load and bind shader
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		
		/***************PASS VALUES INTO THE SHADER*********************/
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
		// Pass in a uniform with the slot the texture is bound to. 
		shader.SetUniform1i("u_Texture", 0);
		Texture texture("res/textures/yellow-texture.jpg");
		// Bind the texture
		texture.Bind();
		/***********************************************************/
		/* Unbind everything for vertex array demostration */

		// Create a renderer
		Renderer renderer;

		// IMGUI CODE
		// Setup ImGui binding
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		/************************************************************************/
		// Create a camera
		Camera camera(window);
		// Enable mouse input for the camera
		camera.enableMouseInput();
		/************************************************************************/



		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();
			float currentFrame = glfwGetTime();
			
			// TODO: Impelemnt Delta time 
			
			// IMGUI Frame can be put anywhere you want aslong as the imgui frame is in between them
			ImGui_ImplGlfwGL3_NewFrame();

			// Start polling for continuous input
			camera.processInput();

			//Keep the matrices updated depending on mouse position
			glm::mat4 mvp = proj * camera.GetViewMatrix() * model;
			// SET the projection matrix in the shader
			shader.SetUniformMat4f("u_MVP", mvp);

			// Draw the models, I should probably have the Models have their own draw function and call render on them
			for (unsigned int i = 0; i < meshes.size(); i++)
			{
				// Renderer takes in a vertex array, Index buffer and a shader to draw stuff
				renderer.Draw(meshes[i]->VA(), meshes[i]->IB(), shader);
			}
			
			// Our GUI solution
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