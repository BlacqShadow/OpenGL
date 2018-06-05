#pragma once

#include <iostream>
#include "Scene.h"
#include <string.h>
#include "Shader.h"
#include "Model.h"
#include "Camera.h"

namespace scene {
	
	class SceneTest : public Scene
	{
	public:
		SceneTest();
		~SceneTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		// Scene Models and Shaders
		Model m_Object;
		Model m_Light;
		Shader m_ObjectShader;
		Shader m_LightShader;

		// Object Matrices
		glm::mat4 m_ObjectModel;
		glm::mat4 m_LightModel;
		glm::mat4 m_Proj;
		glm::mat4 m_Normal;
		glm::mat4 m_View;

		// Object uniforms
		glm::vec3 m_LightPos;
		glm::vec4 m_OAmbientColor;
		glm::vec4 m_ODiffuseColor;
		glm::vec4 m_OSpecularColor;
		glm::vec4 m_LAmbientColor;
		glm::vec4 m_LDiffuseColor;
		glm::vec4 m_LSpecularColor;

		// Camera and renderer
		Renderer renderer;
		Camera camera;
		
	};
}