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

	class SceneLMaps : public Scene
	{
	public:
		SceneLMaps(GLFWwindow* window);
		~SceneLMaps();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		// Scene Models and Shaders
		Model m_Object;
		Model m_LightModel;
		Model m_Skybox;
		Shader m_ObjectShader;
		Shader m_LightShader;
		Shader m_CubeMapShader;
		std::vector<Mesh*> m_ObjectMeshes;

		const unsigned int m_NumLights = 4;

		Texture m_DiffTex;
		Texture m_SpecTex;
		Texture m_SkyBoxTex;

		glm::vec3 m_CubePositions[10] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		glm::vec4 m_LightPos[4] = {
			glm::vec4(0.7f, 0.2f, 2.0f, 1.0f),
			glm::vec4(2.3f, -3.3f, -4.0f, 1.0f),
			glm::vec4(-4.0f, 2.0f, -12.0f, 1.0f),
			glm::vec4(0.0f, 0.0f, -3.0f, 1.0f)
		};

		// Object Matrices
		glm::mat4 m_ObjectModel;
		glm::mat4 m_LightModelMat;
		glm::mat4 m_Proj;
		glm::mat4 m_Normal;
		glm::mat4 m_View;
		glm::mat4 m_SkyBoxModel;

		// Object uniforms
		glm::vec4 m_LightDir;
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