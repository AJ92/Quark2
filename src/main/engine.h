#ifndef ENGINE_H
#define ENGINE_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include "base/gfx/vulkan/vulkan.h"
#include "base/systems/scriptsystem.h"
#include "base/comp/management/entitymanagement.h"

#include <chrono>
#include <memory>
//#include <audio.h>


namespace quark{

	// has to be destroyed before script system for dtor order (script comps destroy within script interpreter)
	static std::shared_ptr<ComponentManagement> mComponentManagement;
	static std::shared_ptr<EntityManagement> mEntityManagement;

	class Engine
	{
	public:
		Engine();
		~Engine();

		void run();

		std::shared_ptr<Vulkan> getVulkanRenderer();

	private:
		bool isDebug;

		bool preInit();
		bool initAllcoators();
		bool initComponentManagement();
		bool initWindow();
		bool initAudio();
		bool initScripting();

		//VULKAN INIT
		bool initVulkan();
		static void onWindowResized(GLFWwindow* window, int width, int height);

		bool postInit();
		bool mainLoop();
		bool cleanUp();

		//diagnostics... probably better to put it in an extra system!
		void initDiagnostics();
		void updateDiagnostics();

		//tests
		void testScripts();

		GLFWwindow * mWindow;

		int mWindowWidth;
		int mWindowHeight;

		std::chrono::time_point<std::chrono::high_resolution_clock> mFrameStart, mFrameEnd;
		std::chrono::duration<double> mCumFrameTimeElapsed;
		uint64_t mCumFrameCount = 0;
		uint64_t mCurrentFrame = 0;

		//std::shared_ptr<Audio> mAudio;
		std::shared_ptr<Vulkan> mVulkan;
		std::shared_ptr<ScriptSystem> mScriptSystem;

	};
};

#endif // ENGINE_H
