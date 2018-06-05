#pragma once

#include <iostream>
#include <string.h>
#include "Scene.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

#include "imgui/imgui.h"

namespace scene {
	
	class SceneTest : public Scene
	{
	public:
		SceneTest(GLFWwindow* window);
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
		std::vector<Mesh*> m_ObjectMeshes;
		std::vector<Mesh*> m_LightMeshes; 

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
		float m_Shine;

		// Camera and renderer
		Renderer m_Renderer;
		Camera m_Camera;
		
	};
}