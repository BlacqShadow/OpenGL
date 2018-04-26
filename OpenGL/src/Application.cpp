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

// 3D object loading
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <vector>




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

		/***************************************ASSIMP OBJ LOADING*******************************************/
		// Variables to store the extracted data
		std::vector<unsigned int> indices;
		std::vector<glm::vec3> vertices;
		std::vector <glm::vec2> uvs;
		std::vector<glm::vec3> normals;

		Assimp::Importer importer;
		// Read the obj file in using assimp
		const aiScene* scene = importer.ReadFile("res/models/cube.obj", 0);
		if (!scene)
		{
			std::cout << "Error: " << importer.GetErrorString() << std::endl;

		}
		// get a pointer to the mesh
		const aiMesh* mesh = scene->mMeshes[0];

		// Increase the capacity of the vector to store n number of vertices
		vertices.reserve(mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			aiVector3D pos = mesh->mVertices[i];
			vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));
		}
		// Fill texture co-ordinates
		uvs.reserve(mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			aiVector3D UVW = mesh->mTextureCoords[0][i];
			uvs.push_back(glm::vec2(UVW.x, UVW.y));
		}

		// Fill vertex normals
		normals.reserve(mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			aiVector3D n = mesh->mNormals[i];
			normals.push_back(glm::vec3(n.x, n.y, n.z));
		}

		// Fill face indices
		indices.reserve(3 * mesh->mNumFaces);
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			// Assume the model has only triangles.
			indices.push_back(mesh->mFaces[i].mIndices[0]);
			indices.push_back(mesh->mFaces[i].mIndices[1]);
			indices.push_back(mesh->mFaces[i].mIndices[2]);
		}

		// The idea is to take out the verticies from glm::vec3 type and put it in a simple array
		std::vector<float> linearVerticies;
		for (int i = 0; i < vertices.size(); i++)
		{
			linearVerticies.push_back(vertices[i].x);
			linearVerticies.push_back(vertices[i].y);
			linearVerticies.push_back(vertices[i].z);
			linearVerticies.push_back(uvs[i].x);
			linearVerticies.push_back(uvs[i].y);
			
		}
		

		/*****************************************************************************************************/
		// First two are Vertex positions,
		// Next Two(same lines) - texture co-ordinates. (0.0 start, 1.0 finish)(Starts bottom left)
		//float positions[] = {
		//	// Positions        Tex-Coord
		//	-0.5f,  0.5f, 0.0f,	0.0f, 1.0f,// 0
		//	 0.5f,  0.5f, 0.0f,	1.0f, 1.0f,// 1
		//	 0.5f, -0.5f, 0.0f,	1.0f, 0.0f,// 2
		//	-0.5f, -0.5f, 0.0f,	0.0f, 0.0f // 3
		//};

		/* To prevent vertex duplication, using indices to refer to the vertex we need to draw a square. */
		/*unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};*/
		
		/* BLENDING */
		// Enable blending
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// Enable Depth buffer testing
		GLCall(glEnable(GL_DEPTH_TEST));

		// Create a Vertex Array Object for buffer vertices
		VertexArray va;
		// Create a Vertex Array Object for buffer texture co-ordinates
		VertexArray texVA;

		
		// Create a Vertex buffer
		VertexBuffer vb(&linearVerticies[0], linearVerticies.size() * sizeof(float));
		// Create another vertex buffer for texture coordinates
		//VertexBuffer vbTex(&uvs, uvs.size() * sizeof(glm::vec2));

		// Create the layout of the Verticies buffer
		VertexBufferLayout verteciesLayout;
		// Tell the layout that each vertex is made out of two floats
		verteciesLayout.Push<float>(3);
		verteciesLayout.Push<float>(2);

		// Create the layout of the Tex coord buffer
		//VertexBufferLayout texLayout;
		//texLayout.Push<float>(2);


		// Add the buffer to the Vertex Array object
		// Add the layout to the VAO
		va.AddBuffer(vb, verteciesLayout);
		//texVA.AddBuffer(vbTex, texLayout);

		IndexBuffer ib(&indices[0], indices.size());

		// Using a perspective matrix instead
		glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

		// Creating a model matrix
		glm::mat4 model;
		
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
		// Pass in a uniform with the slot the texture is bound to. 
		shader.SetUniform1i("u_Texture", 0);
		Texture texture("res/textures/yellow-texture.jpg");

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
		// Enable mouse input for the camera
		camera.enableMouseInput();
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

			// Start polling for continuous input
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