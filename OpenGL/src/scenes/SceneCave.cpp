#include "SceneCave.h"
#include <iostream>
#include "imgui/imgui.h"

namespace scene {

	SceneCave::SceneCave(GLFWwindow* window)
		:m_Skybox("res/models/cube-flat.obj"), m_SkyTexture("res/textures/moon/", "cubemap"), m_SkyBoxShader("res/shaders/SkyBox.glsl"),
		m_Proj(glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f)), m_SkyBoxModel(glm::mat4(1.0f)),
		m_Camera(window)
	{
		m_SkyBoxShader.Bind();
		m_SkyTexture.Bind(0);
		m_SkyBoxShader.SetUniform1i("u_SkyBox", 0);
		m_Camera.enableMouseInput();
	}

	SceneCave::~SceneCave()
	{
		std::cout << "Scene Cave Destructor called" << std::endl;
	}

	void SceneCave::OnUpdate(float deltaTime)
	{
		m_Camera.processInput();
		m_SkyBoxShader.Bind();
		m_SkyBoxShader.SetUniformMat4f("u_MVP", m_Proj * glm::mat4(glm::mat3(m_Camera.GetViewMatrix())) * m_SkyBoxModel);
	}

	void SceneCave::OnRender()
	{
		m_Renderer.Clear();
		GLCall(glDepthMask(GL_FALSE));
		m_Renderer.Draw(m_Skybox, m_SkyBoxShader);
		GLCall(glDepthMask(GL_TRUE));
	}

	void SceneCave::OnImGuiRender()
	{
		//ImGui::ShowDemoWindow();
		ImGui::Text("Camera Co-ordinates: ");
		ImGui::Text("X: %.1f | Y: %.1f | Z: %.1f", Camera::m_CameraPosition.x, Camera::m_CameraPosition.y, Camera::m_CameraPosition.z);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
