#pragma once
#include <iostream>
#include <thread>
#include <imgui/imgui.h>

namespace utilities {
	class LoadingScreen
	{
	public:
		LoadingScreen()
			:worker(&LoadingScreen::PrintLoading, this), working(true)
		{

		}
		~LoadingScreen()
		{
			working = false;
			worker.join();
			std::cout << "Scene Finished Loading";
		}
	private:
		inline void PrintLoading() const
		{ 
			while (working)
			{
				std::cout << "Loading Scene...\n";
				//std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}
		};

	private:
		std::thread worker;
		bool working;
		std::thread::id m_ID;
	};
}