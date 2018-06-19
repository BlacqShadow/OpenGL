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
#include "scenes\SceneLMaps.h"
#include "scenes/SceneTest.h"
#include "scenes/SceneCave.h"

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

		// IMGUI CODE
		// Setup ImGui binding
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		//Texture tex("res/textures/test.png");
		//scene::SceneLMaps myScene(window);
		scene::Scene* myScene = new scene::SceneCave(window);


		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			//renderer.Clear();
			float currentFrame = (float)glfwGetTime();
			
			// TODO: Impelemnt Delta time , currently used for the camera class
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			
			// IMGUI Frame can be put anywhere you want aslong as the imgui frame is in between them
			ImGui_ImplGlfwGL3_NewFrame();
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("Scenes"))
				{
					if (ImGui::MenuItem("Simple Lighting")) { 
						delete myScene; 
						myScene = new scene::SceneTest(window);
					}
					if (ImGui::MenuItem("Model & Lights & Skybox")) {
						delete myScene;
						myScene = new scene::SceneLMaps(window);
					}
					if (ImGui::MenuItem("Cave")) {
						delete myScene;
						myScene = new scene::SceneCave(window);
					}
					ImGui::EndMenu();
				}
				
				ImGui::EndMainMenuBar();
			}
			myScene->OnUpdate(0.0f);
			myScene->OnRender();
			myScene->OnImGuiRender();

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
	std::cout << "Press any key to exit...";
	std::cin.get();
	return 0;
}