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

// GLM Math Library
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

// Math-Time Library Testing
#include <chrono>

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Switch to OpenGL core profile */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
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
			-0.5f, -0.5f, 0.0f, 0.0f,// 0
			 0.5f, -0.5f, 1.0f, 0.0f,// 1
			 0.5f,  0.5f, 1.0f, 1.0f,// 2
			-0.5f,  0.5f, 0.0f, 1.0f // 3
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
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));
		// Create the layout of the buffer
		VertexBufferLayout layout;
		// Tell the layout that each vertex is made out of two floats
		layout.Push<float>(2);
		layout.Push<float>(2);	// For the texture co-ordinates
		// Add the buffer to the Vertex Array object
		// Add the layout to the VAO
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		// Creating the projection matrix (specify a 4:3 ratio) 
		// We will use this matrix to render to our 4:3 aspect ration window 
		// Note: Orthographic Matrix (Orthographic Projection) does not use a perspective projection (Objects get small as they go away) 
		glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.125f, 1.125f, -1.0f, 1.0f);

		/******** MATH LIBRARY TESTING ************************************/
		// Calculate transformation
		auto t_start = std::chrono::high_resolution_clock::now();
		
	
		
		//printf("%f, %f, %f, %f\n", result.x, result.y, result.z, result.w);
		//std::cout << std::fixed << result.x << ", " << result.y << ", " << result.z << ", " << result.w << std::endl;
		/*******************************************************************/

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
		// Pass in a uniform with the slot the texture is bound to. 
		shader.SetUniform1i("u_Texture", 0);
		Texture texture("res/textures/valencia.png");
		// SET the projection matrix in the shader
		shader.SetUniformMat4f("u_MVP", proj);
		/*************************Math Library Testing*******************************/
		std::cout << "Setting the rotation matrix in the shader" << std::endl;
		
		/**************************/

		/* Unbind everything for vertex array demostration */
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();
		texture.Bind();
		// Bind the texture

		/* Variables to change the color of the rectangle */
		float increment = 0.05f;
		float r = 0.8f;

		Renderer renderer;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			/* Bind all the buffers again before issuing a draw call */
			shader.Bind();
			// set the colors on the fly

			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
	
			//MATH LIBRARY CODE
			auto t_now = std::chrono::high_resolution_clock::now();
			float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

			glm::mat4 trans = glm::mat4(1.0f);
			glm::vec4 result = trans * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			trans = glm::rotate(trans, time * glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			shader.SetUniformMat4f("u_Rotation", trans);

			/*************************************************************************************/
			// Use the renderer to draw stuff 
			renderer.Draw(va, ib, shader);

			/* Change the r values as the program loops*/
			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;
			r += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}