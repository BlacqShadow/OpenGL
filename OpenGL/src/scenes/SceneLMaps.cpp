#include "SceneLMaps.h"
#include <string.h>


namespace scene {
	SceneLMaps::SceneLMaps(GLFWwindow* window)
		:m_Object("res/models/Lantern.obj"), m_ObjectShader("res/shaders/LightMap.glsl"),m_Skybox("res/models/cube-flat.obj") ,m_CubeMapShader("res/shaders/SkyBox.glsl"), m_Camera(window), 
		m_DiffTex("res/textures/container2.png"), m_SpecTex("res/textures/specular.png"), m_SkyBoxTex("res/textures/moon/", "cubemap"), m_LightModel("res/models/LightBulb.obj"), m_LightShader("res/shaders/Light.glsl")
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

		// Set uniform in the object shader
		m_CubeMapShader.Bind();
		m_SkyBoxTex.Bind(0);
		m_CubeMapShader.SetUniform1i("u_SkyBox", 0);
		
		// Send uniform data to the object shader
		m_ObjectShader.Bind();
		m_ObjectModel = glm::scale(m_ObjectModel, glm::vec3(1.0f, 1.0f, 1.0f));
		m_ObjectShader.SetUniformMat4f("u_Model", m_ObjectModel);
		m_ObjectShader.SetUniformMat4f("u_Normal", m_Normal);
		
		// Bind the texture to a slot
		//m_DiffTex.Bind(0);
		// Tell the GPU, this is the slot this texture is bound to 
		//m_ObjectShader.SetUniform1i("u_Material.Texture_Diffuse0", 0);

		//m_SpecTex.Bind(1);
		//m_ObjectShader.SetUniform1i("u_Material.Texture_Specular0", 1);
		m_ObjectShader.SetUniform1f("u_Material.shine", m_Shine);
		

		m_ObjectShader.SetUniform4f("u_DirLight.ambient", m_LAmbientColor);
		m_ObjectShader.SetUniform4f("u_DirLight.diffuse", m_LDiffuseColor);
		m_ObjectShader.SetUniform4f("u_DirLight.specular", m_LSpecularColor);
		m_ObjectShader.SetUniform4f("u_DirLight.lightDir", m_LightDir);


		// Point light calculations
		// Set the position for all the point lights
		for (int i = 0; i < m_NumLights; i++)
		{
			
			m_ObjectShader.SetUniform4f("u_PointLight[" + std::to_string(i) + "].position", m_LightPos[i]);
			m_ObjectShader.SetUniform4f("u_PointLight[" + std::to_string(i) + "].ambient", m_LAmbientColor);
			m_ObjectShader.SetUniform4f("u_PointLight[" + std::to_string(i) + "].diffuse", m_LDiffuseColor);
			m_ObjectShader.SetUniform4f("u_PointLight[" + std::to_string(i) + "].specular", m_LSpecularColor);
			m_ObjectShader.SetUniform1f("u_PointLight[" + std::to_string(i) + "].constant", 1.0f);
			m_ObjectShader.SetUniform1f("u_PointLight[" + std::to_string(i) + "].linear", 0.09f);
			m_ObjectShader.SetUniform1f("u_PointLight[" + std::to_string(i) + "].quadratic", 0.032f);
			

		}
		
		// Delete if the above works 
		/*m_ObjectShader.SetUniform4f("u_PointLight.position", m_LightPos);
		m_ObjectShader.SetUniform4f("u_PointLight.ambient", m_LAmbientColor);
		m_ObjectShader.SetUniform4f("u_PointLight.diffuse", m_LDiffuseColor);
		m_ObjectShader.SetUniform4f("u_PointLight.specular", m_LSpecularColor);
		m_ObjectShader.SetUniform1f("u_PointLight.constant", 1.0f);
		m_ObjectShader.SetUniform1f("u_PointLight.linear", 0.09f);
		m_ObjectShader.SetUniform1f("u_PointLight.quadratic", 0.032f);*/
		

		// Enable camera mouse input
		m_Camera.enableMouseInput();
	}
	SceneLMaps::~SceneLMaps()
	{
		std::cout << "\n\n scene LightMap Destructor called " << std::endl;
	}
	void SceneLMaps::OnUpdate(float deltaTime)
	{
		m_Camera.processInput();
		// Update the light model 
		m_View = m_Camera.GetViewMatrix();

		// Update the light Model
		m_LightShader.Bind();
		
		
		
		// Update the skybox model
		m_CubeMapShader.Bind(); 
		m_CubeMapShader.SetUniformMat4f("u_MVP", m_Proj * glm::mat4(glm::mat3(m_Camera.GetViewMatrix())) * m_SkyBoxModel);

		// Update the Object Model
		m_ObjectShader.Bind();
		m_ObjectShader.SetUniformMat4f("u_MVP", m_Proj *  m_View * m_ObjectModel);
		m_ObjectShader.SetUniform3f("u_CameraPos", Camera::m_CameraPosition);


		// Update light positions every frame 
		for (int i = 0; i < m_NumLights; i++)
		{
			m_ObjectShader.SetUniform4f("u_PointLight[" + std::to_string(i) + "].position", m_LightPos[i]);
		}
	}
	void SceneLMaps::OnRender()
	{
		m_Renderer.Clear();
		
		// Disable the depth mask always enable drawing
		GLCall(glDepthMask(GL_FALSE));
		
		m_Renderer.Draw(m_Skybox, m_CubeMapShader);
		GLCall(glDepthMask(GL_TRUE));
		m_Renderer.Draw(m_Object, m_ObjectShader);
		for (unsigned int i = 0; i < m_NumLights; i++)
		{
			m_LightShader.Bind();
			m_LightModelMat = glm::mat4(1.0f);
			m_LightModelMat = glm::translate(m_LightModelMat, glm::vec3(m_LightPos[i]));
			m_LightModelMat = glm::scale(m_LightModelMat, glm::vec3(0.005f));
			m_LightShader.SetUniformMat4f("u_MVP", m_Proj * m_View * m_LightModelMat);
			m_LightShader.SetUniform4f("u_LightColor", 1.0f, 1.0f, 1.0f, 1.0f);
			m_Renderer.Draw(m_LightModel, m_LightShader);
		}

	}
	void SceneLMaps::OnImGuiRender()
	{
		//ImGui::ShowDemoWindow();
		ImGui::Text("Camera Co-ordinates: ");
		ImGui::Text("X: %.1f | Y: %.1f | Z: %.1f", Camera::m_CameraPosition.x, Camera::m_CameraPosition.y, Camera::m_CameraPosition.z);
		ImGui::SliderFloat3("Light Pos 1", glm::value_ptr(m_LightPos[0]), -15.0f, 15.0f);
		ImGui::SliderFloat3("Light Pos 2", glm::value_ptr(m_LightPos[1]), -15.0f, 15.0f);
		ImGui::SliderFloat3("Light Pos 3", glm::value_ptr(m_LightPos[2]), -15.0f, 15.0f);
		ImGui::SliderFloat3("Light Pos 4", glm::value_ptr(m_LightPos[3]), -15.0f, 15.0f);
		
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}