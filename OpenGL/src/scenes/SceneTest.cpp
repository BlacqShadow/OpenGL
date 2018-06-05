#include "SceneTest.h"


namespace scene {
	SceneTest::SceneTest(GLFWwindow* window)
		:m_Object("res/models/cube-flat.obj"), m_Light("res/models/Lightbulb.obj"), m_ObjectShader("res/shaders/Basic.glsl"), m_LightShader("res/shaders/Light.glsl"), m_Camera(window)
	{
		// Initialize transformation matrices
		m_Proj = glm::perspective(glm::radians(75.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
		m_Normal = glm::transpose(glm::inverse(m_LightModel));

		// Retrive the meshes from the models
		m_ObjectMeshes = m_Object.GetMeshes();
		m_LightMeshes = m_Light.GetMeshes();

		// Set the position of the light
		m_LightPos = glm::vec3(1.2f, 1.0f, 2.0f);

		// Object Material
		m_OAmbientColor = glm::vec4(1.0f, 0.5f, 0.31f, 1.0f);
		m_ODiffuseColor = glm::vec4(1.0f, 0.5f, 0.31f, 1.0f);
		m_OSpecularColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		m_Shine = 32.0f;

		// Light Properties
		m_LAmbientColor  = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		m_LDiffuseColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		m_LSpecularColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		// Send uniform data to the light shader
		m_LightShader.Bind();
		m_LightShader.SetUniform4f("u_LightColor", 1.0f, 1.0f, 1.0f, 1.0f);


		// Send uniform data to the object shader
		m_ObjectShader.Bind();
		m_ObjectShader.SetUniformMat4f("u_Model", m_ObjectModel);
		m_ObjectShader.SetUniformMat4f("u_Normal", m_Normal);
		m_ObjectShader.SetUniform4f("u_Material.ambient", m_OAmbientColor);
		m_ObjectShader.SetUniform4f("u_Material.diffuse", m_ODiffuseColor);
		m_ObjectShader.SetUniform4f("u_Material.specular", m_OSpecularColor);
		m_ObjectShader.SetUniform1f("u_Material.shine", m_Shine);

		m_ObjectShader.SetUniform4f("u_Light.ambient", m_LAmbientColor);
		m_ObjectShader.SetUniform4f("u_Light.diffuse", m_LDiffuseColor);
		m_ObjectShader.SetUniform4f("u_Light.specular", m_LSpecularColor);
		m_ObjectShader.SetUniform3f("u_Light.lightPos", m_LightPos);

		// Enable camera mouse input
		m_Camera.enableMouseInput();
	}
	SceneTest::~SceneTest()
	{

	}
	void SceneTest::OnUpdate(float deltaTime)
	{
		m_Camera.processInput();
		// Update the light model 
		m_View = m_Camera.GetViewMatrix();
		m_LightModel = glm::translate(glm::mat4(1.0f), m_LightPos);
		m_LightModel = glm::scale(m_LightModel, glm::vec3(0.005f));
		m_LightShader.Bind();
		m_LightShader.SetUniformMat4f("u_MVP", m_Proj * m_View * m_LightModel);

		// Update the Object Model
		m_ObjectShader.Bind();
		m_ObjectShader.SetUniformMat4f("u_MVP", m_Proj *  m_View * m_ObjectModel);
		m_ObjectShader.SetUniform3f("u_Light.lightPos", m_LightPos);
		m_ObjectShader.SetUniform3f("u_CameraPos", Camera::m_CameraPosition);


	}
	void SceneTest::OnRender()
	{
		m_Renderer.Clear();

		// Draw the models, I should probably have the Models have their own draw function and call render on them
		for (unsigned int i = 0; i < m_ObjectMeshes.size(); i++)
		{
			// Renderer takes in a vertex array, Index buffer and a shader to draw stuff
			m_Renderer.Draw(m_ObjectMeshes[i]->VA(), m_ObjectMeshes[i]->IB(), m_ObjectShader);
		}

		for (unsigned int i = 0; i < m_LightMeshes.size(); i++)
		{
			// Renderer takes in a vertex array, Index buffer and a shader to draw stuff
			m_Renderer.Draw(m_LightMeshes[i]->VA(), m_LightMeshes[i]->IB(), m_LightShader);
		}
	}
	void SceneTest::OnImGuiRender()
	{
		//ImGui::ShowDemoWindow();
		ImGui::Text("Camera Co-ordinates: ");
		ImGui::Text("X: %.1f | Y: %.1f | Z: %.1f", Camera::m_CameraPosition.x, Camera::m_CameraPosition.y, Camera::m_CameraPosition.z);
		ImGui::SliderFloat3("Light Position", glm::value_ptr(m_LightPos), -10.0f, 10.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}