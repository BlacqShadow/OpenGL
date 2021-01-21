#include "SceneCave.h"
#include <iostream>
#include "imgui/imgui.h"

namespace scene {

	

	SceneCave::SceneCave(GLFWwindow* window)
		:m_Skybox("res/models/cube-flat.obj"), m_SkyTexture("res/textures/moon/", "cubemap"), m_SkyBoxShader("res/shaders/SkyBox.glsl"),
		m_Cave("res/models/Cave.fbx"), m_CaveShader("res/shaders/Cave.glsl"), m_Light("res/models/Lightbulb.obj"), m_LightShader("res/shaders/Light.glsl"),
		m_Proj(glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f)), m_CaveNormal(glm::transpose(glm::inverse(m_CaveModel))),
		m_Camera(window), t_CavePosition(glm::vec3(0))
	{
		// Configure skybox
		m_SkyBoxShader.Bind();
		m_SkyTexture.Bind();
		m_SkyBoxShader.SetUniform1i("u_SkyBox", 0);
		
		// Enable mouse input
		m_Camera.EnableMouseInput();
		// Set cave uniforms 
		SetupCave();
		// Set light uniforms 
		SetupLights();
	}

	SceneCave::~SceneCave()
	{
		std::cout << "\n\nScene Cave Destructor called" << std::endl;
	}

	void SceneCave::OnUpdate(float deltaTime)
	{
		// get the view matrix and start processing keyboard input
		m_View = m_Camera.GetViewMatrix();
		m_Camera.processInput();

		// Update skybox shader uniforms 
		m_SkyBoxShader.Bind();
		m_SkyBoxShader.SetUniformMat4f("u_MVP", m_Proj * glm::mat4(glm::mat3(m_View)) * m_SkyBoxModel);

		// Update cave uniforms
		m_CaveShader.Bind();
		glm::quat rotationQuat(glm::vec3(0.0f, 3.14f / 2, 0.0f));
		m_CaveModel = glm::translate(glm::mat4(1.0f), t_CavePosition) * glm::mat4(rotationQuat);
		m_CaveShader.SetUniformMat4f("u_MVP", m_Proj * m_View * m_CaveModel);
		m_CaveShader.SetUniform3f("u_CameraPos", Camera::m_CameraPosition);
		
		// Update light positions every frame 
		for (int i = 0; i < m_NumLights; i++)
		{
			m_CaveShader.SetUniform4f("u_PointLight[" + std::to_string(i) + "].position", m_LightPos[i]);
		}

	}

	void SceneCave::OnRender()
	{
		m_Renderer.Clear();
		GLCall(glDepthMask(GL_FALSE));
		m_Renderer.Draw(m_Skybox, m_SkyBoxShader);
		GLCall(glDepthMask(GL_TRUE));

		// Draw the cave 
		m_Renderer.Draw(m_Cave, m_CaveShader);

		// Draw the lights
		for (unsigned int i = 0; i < m_NumLights; i++)
		{
			m_LightShader.Bind();
			m_LightModel = glm::mat4(1.0f);
			m_LightModel = glm::translate(m_LightModel, glm::vec3(m_LightPos[i]));
			m_LightModel = glm::scale(m_LightModel, glm::vec3(0.005f));
			m_LightShader.SetUniformMat4f("u_MVP", m_Proj * m_View * m_LightModel);
			m_LightShader.SetUniform4f("u_LightColor", 1.0f, 1.0f, 1.0f, 1.0f);
			m_Renderer.Draw(m_Light, m_LightShader);
		}

	}

	void SceneCave::OnImGuiRender()
	{
		//ImGui::ShowDemoWindow();
		ImGui::Text("Camera Co-ordinates: ");
		ImGui::Text("X: %.1f | Y: %.1f | Z: %.1f", Camera::m_CameraPosition.x, Camera::m_CameraPosition.y, Camera::m_CameraPosition.z);
		ImGui::Separator();
		ImGui::DragFloat("Camera Distance", &Camera::m_Distance);
		ImGui::DragFloat3("Focal Point", glm::value_ptr(Camera::m_FocalPoint));
		ImGui::SliderFloat3("Light Pos 1", glm::value_ptr(m_LightPos[0]), -30.0f, 30.0f);
		ImGui::SliderFloat3("Light Pos 2", glm::value_ptr(m_LightPos[1]), -30.0f, 30.0f);
		ImGui::SliderFloat3("Light Pos 3", glm::value_ptr(m_LightPos[2]), -30.0f, 30.0f);
		ImGui::SliderFloat3("Light Pos 4", glm::value_ptr(m_LightPos[3]), -30.0f, 30.0f);
		ImGui::SliderFloat3("Cave Position", glm::value_ptr(t_CavePosition), -30.0f, 30.0f);
		ImGui::Separator();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	void SceneCave::SetupCave()
	{
		// Basic variables 
		 glm::vec4 m_LAmbientColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		 glm::vec4 m_LDiffuseColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		 glm::vec4 m_LSpecularColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		 glm::vec4 m_LightDir = glm::vec4(0.2f, 1.0f, -0.3f, 0.0f);

		m_CaveShader.Bind();
		m_CaveShader.SetUniformMat4f("u_Model", m_CaveModel);
		m_CaveShader.SetUniformMat4f("u_Normal", m_CaveNormal);
		m_CaveShader.SetUniform1f("u_Material.shine", 32.0f);
	

		// Setup the light properties of the object
		m_CaveShader.SetUniform4f("u_DirLight.ambient",  m_LAmbientColor);
		m_CaveShader.SetUniform4f("u_DirLight.diffuse",  m_LDiffuseColor);
		m_CaveShader.SetUniform4f("u_DirLight.specular", m_LSpecularColor);
		m_CaveShader.SetUniform4f("u_DirLight.lightDir", m_LightDir);

		// Point light calculations
		// Set the position for all the point lights
		for (int i = 0; i < m_NumLights; i++)
		{

			m_CaveShader.SetUniform4f("u_PointLight[" + std::to_string(i) + "].position", m_LightPos[i]);
			m_CaveShader.SetUniform4f("u_PointLight[" + std::to_string(i) + "].ambient", m_LAmbientColor);
			m_CaveShader.SetUniform4f("u_PointLight[" + std::to_string(i) + "].diffuse", m_LDiffuseColor);
			m_CaveShader.SetUniform4f("u_PointLight[" + std::to_string(i) + "].specular", m_LSpecularColor);
			m_CaveShader.SetUniform1f("u_PointLight[" + std::to_string(i) + "].constant", 1.0f);
			m_CaveShader.SetUniform1f("u_PointLight[" + std::to_string(i) + "].linear", 0.09f);
			m_CaveShader.SetUniform1f("u_PointLight[" + std::to_string(i) + "].quadratic", 0.032f);


		}

	}

	void SceneCave::SetupLights()
	{
	}

}
