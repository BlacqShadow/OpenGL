#pragma once 
#include <vector>
#include <functional>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
// Base class for all our test scenes 

// New namespace to prevent naming conflicts with assimp 
namespace scene {

	class Scene
	{
	public:
		Scene() {}
		virtual ~Scene() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		// To render the IMGUI stuff
		virtual void OnImGuiRender() {}
	};

	class SceneMenu: public Scene
	{
	public: 
		SceneMenu(Scene*& currentScene, GLFWwindow* window);

		virtual void OnImGuiRender() override;

		// A Method that returns an instance of whatever type that is passed in
		template<typename T>
		void RegisterScene(const std::string name)
		{
			m_Scenes.push_back(std::make_pair(name, [this]() {return new T(m_Window); }));
		}
	private:
		Scene*& m_currentScene;
		GLFWwindow* m_Window;
		std::vector <std::pair<std::string, std::function<Scene*()>>> m_Scenes;

	};


}