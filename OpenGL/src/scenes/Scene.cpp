#include "scenes/Scene.h"
#include "imgui/imgui.h"
#include "util/Timer.h"

namespace scene {

	SceneMenu::SceneMenu(Scene*& currentScene, GLFWwindow* window)
		:m_currentScene(currentScene), m_Window(window)
	{

	}

	void SceneMenu::OnImGuiRender()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Scenes"))
			{
				for (auto& scene : m_Scenes)
				{
					if (ImGui::MenuItem(scene.first.c_str()))
					{
						utilities::LoadingScreen loading;
						if (m_currentScene != this)
						{
							delete m_currentScene;
							m_currentScene = scene.second();
						}
						else
							m_currentScene = scene.second();
					}
				}
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

	

}