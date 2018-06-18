#pragma once 
#include "scenes/Scene.h"
#include "Model.h"
#include "Renderer.h"
#include "Camera.h"


namespace scene {
	class SceneCave : public Scene
	{
	private:
		Model m_Skybox; 
		Texture m_SkyTexture;
		Shader m_SkyBoxShader;
		glm::mat4 m_Proj;
		glm::mat4 m_SkyBoxModel;
		Camera m_Camera;
		Renderer m_Renderer;

	public:
		SceneCave(GLFWwindow* window);
		~SceneCave();
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;

	};
}