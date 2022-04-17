#ifndef ENGINE_H
#define ENGINE_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include "base/gfx/vulkan/vulkan.h"
#include "base/systems/scriptsystem.h"

#include <chrono>
#include <memory>
//#include <audio.h>


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

    //std::shared_ptr<Audio> mAudio;
	std::shared_ptr<Vulkan> mVulkan;
	std::shared_ptr<ScriptSystem> mScriptSystem;
	// has to be after the script system for dtor order (script comps destroy within script interpreter)
	std::shared_ptr<ComponentManagement> mComponentManagement;

	std::chrono::time_point<std::chrono::high_resolution_clock> mFrameStart, mFrameEnd;
	std::chrono::duration<double> mCumFrameTimeElapsed;
	int mCumFrameCount;
	int mCurrentFrame;

};

#endif // ENGINE_H
