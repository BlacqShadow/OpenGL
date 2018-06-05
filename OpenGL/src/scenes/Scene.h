#pragma once 

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
}