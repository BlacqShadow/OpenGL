#include "SceneLMaps.h"


namespace scene {
	SceneLMaps::SceneLMaps(GLFWwindow* window)
		:m_Object("res/models/cube-flat.obj"), m_ObjectShader("res/shaders/LightMap.glsl"), m_Camera(window), 
		m_DiffTex("res/textures/container2.png"), m_SpecTex("res/textures/specular.png")
	{
		// Initialize transformation matrices
		m_Proj = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
		m_Normal = glm::transpose(glm::inverse(m_ObjectModel));

		// Retrive the meshes from the models
		m_ObjectMeshes = m_Object.GetMeshes();

		// Set the position of the light
		m_LightDir = glm::vec4(-0.2f, -1.0f, -0.3f, 0.0f);

		// Object Material
		m_OAmbientColor = glm::vec4(1.0f, 0.5f, 0.31f, 1.0f);
		m_ODiffuseColor = glm::vec4(1.0f, 0.5f, 0.31f, 1.0f);
		m_OSpecularColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		m_Shine = 32.0f;

		// Light Properties
		m_LAmbientColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		m_LDiffuseColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		m_LSpecularColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		// Send uniform data to the object shader
		m_ObjectShader.Bind();
		m_ObjectShader.SetUniformMat4f("u_Model", m_ObjectModel);
		m_ObjectShader.SetUniformMat4f("u_Normal", m_Normal);
		
		// Bind the texture to a slot
		m_DiffTex.Bind(0);
		// Tell the GPU, this is the slot this texture is bound to 
		m_ObjectShader.SetUniform1i("u_Material.Texture_Diffuse0", 0);

		m_SpecTex.Bind(1);
		m_ObjectShader.SetUniform1i("u_Material.Texture_Specular0", 1);
		m_ObjectShader.SetUniform1f("u_Material.shine", m_Shine);
		

		m_ObjectShader.SetUniform4f("u_Light.ambient", m_LAmbientColor);
		m_ObjectShader.SetUniform4f("u_Light.diffuse", m_LDiffuseColor);
		m_ObjectShader.SetUniform4f("u_Light.specular", m_LSpecularColor);
		m_ObjectShader.SetUniform4f("u_Light.lightDir", m_LightDir);

		// Enable camera mouse input
		m_Camera.enableMouseInput();
	}
	SceneLMaps::~SceneLMaps()
	{

	}
	void SceneLMaps::OnUpdate(float deltaTime)
	{
		m_Camera.processInput();
		// Update the light model 
		m_View = m_Camera.GetViewMatrix();

		// Update the Object Model
		m_ObjectShader.Bind();
		m_ObjectShader.SetUniformMat4f("u_MVP", m_Proj *  m_View * m_ObjectModel);
		m_ObjectShader.SetUniform3f("u_CameraPos", Camera::m_CameraPosition);
	}
	void SceneLMaps::OnRender()
	{
		m_Renderer.Clear();
		for (unsigned int i = 0; i < 10; i++)
		{
			m_ObjectModel = glm::mat4(1.0f);
			m_ObjectModel = glm::translate(m_ObjectModel, m_CubePositions[i]);
			float angle = 20.0f * i;
			m_ObjectModel = glm::rotate(m_ObjectModel, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			// Update the matrices
			m_ObjectShader.SetUniformMat4f("u_Model", m_ObjectModel);
			m_Normal = glm::transpose(glm::inverse(m_ObjectModel));
			m_ObjectShader.SetUniformMat4f("u_Normal", m_Normal);
			m_ObjectShader.SetUniformMat4f("u_MVP", m_Proj *  m_View * m_ObjectModel);
			m_Renderer.Draw(m_Object, m_ObjectShader);
		}

	}
	void SceneLMaps::OnImGuiRender()
	{
		//ImGui::ShowDemoWindow();
		ImGui::Text("Camera Co-ordinates: ");
		ImGui::Text("X: %.1f | Y: %.1f | Z: %.1f", Camera::m_CameraPosition.x, Camera::m_CameraPosition.y, Camera::m_CameraPosition.z);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}