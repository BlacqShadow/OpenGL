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

		/*************************MODEL LOADING******************/
		// Mesh load test
		Model test("res/models/cube-flat.obj");
		const std::vector<Mesh*> meshes = test.GetMeshes();
		// Light source sphere load
		Model sphere("res/models/Lightbulb.obj");
		const std::vector<Mesh*> sphereMesh = sphere.GetMeshes();

		/*********************************************************/

		// Using a perspective matrix with a fov of 75
		glm::mat4 proj = glm::perspective(glm::radians(75.0f), screenWidth / screenHeight, 0.1f, 100.0f);

		// Creating a model matrix
		glm::mat4 model;
		
		// Create a light shader
		Shader lightShader("res/shaders/Light.glsl");
		glm::vec3 lightPosition = glm::vec3(1.2f, 1.0f, 2.0f);
		glm::mat4 lightModel = glm::mat4(1.0f);


		// Load and bind shader
		Shader shader("res/shaders/Basic.glsl");
		shader.Bind();
		shader.SetUniformMat4f("u_Model", model);
		// Get a normal matrix to perform operation on normal vectors in world space;
		glm::mat4 normal = glm::transpose(glm::inverse(model));
		shader.SetUniformMat4f("u_Normal", normal);
		
		
		/***************PASS VALUES INTO THE SHADER*********************/
		glm::vec4 ambientColor = glm::vec4(1.0f, 0.5f, 0.31f, 1.0f);
		glm::vec4 diffuseColor = glm::vec4(1.0f, 0.5f, 0.31f, 1.0f);
		glm::vec4 specularColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		float shine = 32.0f;

		// Light Intensities
		glm::vec4 L_ambientColor =  glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		glm::vec4 L_diffuseColor =  glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		glm::vec4 L_specularColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);


		shader.SetUniform4f("u_Material.ambient", ambientColor);
		shader.SetUniform4f("u_Material.diffuse", diffuseColor);
		shader.SetUniform4f("u_Material.specular", specularColor);
		shader.SetUniform1f("u_Material.shine", shine);

		shader.SetUniform4f("u_Light.ambient", L_ambientColor);
		shader.SetUniform4f("u_Light.diffuse", L_diffuseColor);
		shader.SetUniform4f("u_Light.specular", L_specularColor);
		shader.SetUniform3f("u_Light.lightPos", lightPosition);

		
		
		// Pass in a uniform with the slot the texture is bound to. 
		//shader.SetUniform1i("u_Texture", 0);
		// This is how you create a texture
		//Texture texture("res/textures/yellow-texture.jpg");
		// Bind the texture
		// texture.Bind();
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
			float currentFrame = (float)glfwGetTime();
			
			// TODO: Impelemnt Delta time 
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			
			// IMGUI Frame can be put anywhere you want aslong as the imgui frame is in between them
			ImGui_ImplGlfwGL3_NewFrame();

			// Start polling for continuous input
			camera.processInput();
			glm::mat4 view = camera.GetViewMatrix();


			// Draw light source
			for (unsigned int i = 0; i < sphereMesh.size(); i++)
			{
				// Renderer takes in a vertex array, Index buffer and a shader to draw stuff
				renderer.Draw(sphereMesh[i]->VA(), sphereMesh[i]->IB(), lightShader);
			}
			//Keep the matrices updated depending on mouse position
			glm::mat4 mvp = proj * view * lightModel;
			//lightModel = glm::translate(lightModel, -lightPosition);
			/***************Set Uniforms***************************/
			// SET the projection matrix in the shader
			lightShader.SetUniformMat4f("u_MVP", mvp);
			lightShader.SetUniform4f("u_LightColor", 1.0f, 1.0f, 1.0f, 1.0f);
			// Dynamically adjust the posiition of the light
			lightModel = glm::translate(model, lightPosition);
			lightModel = glm::scale(lightModel, glm::vec3(0.005f));
			/*******************************************************/
						

			// Draw the models, I should probably have the Models have their own draw function and call render on them
			for (unsigned int i = 0; i < meshes.size(); i++)
			{
				// Renderer takes in a vertex array, Index buffer and a shader to draw stuff
				renderer.Draw(meshes[i]->VA(), meshes[i]->IB(), shader);
			}
			//Keep the matrices updated depending on mouse position
			mvp = proj * view * model;
			/***************Set Uniforms***************************/
			// SET the projection matrix in the shader
			shader.SetUniformMat4f("u_MVP", mvp);
			shader.SetUniform3f("u_Light.lightPos", lightPosition);
			shader.SetUniform3f("u_CameraPos", Camera::m_CameraPosition);
			/*******************************************************/

			// Our GUI solution
			{
				//ImGui::ShowDemoWindow();
				ImGui::Text("Camera Co-ordinates: ");
				ImGui::Text("X: %.1f | Y: %.1f | Z: %.1f", Camera::m_CameraPosition.x, Camera::m_CameraPosition.y, Camera::m_CameraPosition.z);
				ImGui::SliderFloat3("Light Position", glm::value_ptr(lightPosition), -10.0f, 10.0f);
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