#pragma once 
#include "scenes/Scene.h"
#include "Model.h"
#include "Renderer.h"
#include "Camera.h"


namespace scene {
	class SceneCave : public Scene
	{
	private:
		// Skybox Model
		Model m_Skybox; 
		Texture m_SkyTexture;
		Shader m_SkyBoxShader;

		// Cave Model 
		Model m_Cave;
		Shader m_CaveShader;

		// Light Model
		Model m_Light;
		Shader m_LightShader;

		// Matrices
		glm::mat4 m_Proj;
		glm::mat4 m_SkyBoxModel;
		glm::mat4 m_CaveModel;
		glm::mat4 m_View;
		glm::mat4 m_CaveNormal;
		glm::mat4 m_LightModel;
		
		Camera m_Camera;
		Renderer m_Renderer;

		glm::vec4 m_LightPos[4] = {
			glm::vec4(0.7f, 0.2f, 2.0f, 1.0f),
			glm::vec4(2.3f, -3.3f, -4.0f, 1.0f),
			glm::vec4(-4.0f, 2.0f, -12.0f, 1.0f),
			glm::vec4(0.0f, 0.0f, -3.0f, 1.0f)
		};
		const unsigned int m_NumLights = 4;

		void SetupCave();
		void SetupLights();
	public:
		SceneCave(GLFWwindow* window);
		~SceneCave();
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;

	};
}