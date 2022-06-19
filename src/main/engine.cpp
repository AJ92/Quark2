#include "engine.h"
#include <iostream>

namespace quark {

	Engine::Engine() :
		mWindow(0)
	{
	}

	Engine::~Engine() {
		// needed to clean up before the systems get destroyed
		mEntityManagement.reset();
		mComponentManagement.reset();
	}

	void Engine::run() {
		preInit();
		initAllcoators();
		initComponentManagement();
		initWindow();
		initAudio();
		initScripting();
		initVulkan();
		initDiagnostics();
		postInit();
		mainLoop();
		cleanUp();
	}

	std::shared_ptr<Vulkan> Engine::getVulkanRenderer() {
		return mVulkan;
	}


	///////////////////////////////////////////////
	//
	//		PRIVATE

	bool Engine::preInit() {
	#ifndef _DEBUG
	#	if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
		//FreeConsole();
	#	endif
		std::cout << "RELEASE" << std::endl;
		isDebug = false;
	#else
		std::cout << "DEBUG" << std::endl;
		isDebug = true;
	#endif

		return true;
	}

	bool Engine::initAllcoators() {
		return true;
	}

	bool Engine::initComponentManagement() {
		mComponentManagement = std::make_shared<ComponentManagement>();
		return true;
	}

	bool Engine::initWindow() {
		glfwInit();

		mWindowWidth = 800;
		mWindowHeight = 600;

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, "Quark2", nullptr, nullptr);
		glfwSetWindowUserPointer(mWindow, this);
		glfwSetWindowSizeCallback(mWindow, Engine::onWindowResized);

		return true;
	}

	bool Engine::initVulkan() {
		mVulkan = std::make_shared<Vulkan>(mWindow, mWindowWidth, mWindowHeight, isDebug);
		return true;
	}

	void Engine::onWindowResized(GLFWwindow* window, int width, int height) {
		if (width == 0 || height == 0) return;

		Engine* engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
		engine->getVulkanRenderer()->windowResize(width, height);
	}

	bool Engine::initAudio() {
		//_audio = std::make_shared<Audio>(isDebug);
		return true;
	}

	bool Engine::initScripting() {
		mScriptSystem = std::make_shared<ScriptSystem>(mComponentManagement);
		return true;
	}

	//AUDIO TEST
	bool Engine::postInit() {
		std::string filename2 = "C:/Code/VS/engine-main/engine/build/Debug/test3.flac";
		//_audio->processEvent(AudioEventType::start, &_audio->getSound(filename2), true);

		//test_p_math();
		//test_allocs();
		testScripts();

		return true;
	}

	bool Engine::mainLoop() {
		mScriptSystem->init();

		while (!glfwWindowShouldClose(mWindow)) {
			glfwPollEvents();
			mScriptSystem->update();
			mVulkan->updateUniformBuffer();
			mVulkan->drawFrame();
			updateDiagnostics();
		}
		return true;
	}

	bool Engine::cleanUp() {
		mVulkan->cleanUp();
		glfwDestroyWindow(mWindow);
		glfwTerminate();
		return true;
	}

	void Engine::initDiagnostics() {
		mCurrentFrame = 0;
		mCumFrameCount = 0;
		mCumFrameTimeElapsed = std::chrono::duration<double>(0.0);
		mFrameStart = std::chrono::high_resolution_clock::now();
	}

	void Engine::updateDiagnostics() {
		mFrameEnd = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed_seconds = mFrameEnd - mFrameStart;
		mCumFrameTimeElapsed += elapsed_seconds;
		mFrameStart = mFrameEnd;

		mCumFrameCount += 1;

		if (mCumFrameTimeElapsed.count() > 1.0) {
			//print fps

			std::cout << "mspf = " << mCumFrameTimeElapsed.count() / mCumFrameCount <<
			"  fps = " << 1.0 / mCumFrameTimeElapsed.count() * mCumFrameCount << std::endl;

			mCumFrameTimeElapsed = std::chrono::duration<double>(0.0);
			mCumFrameCount = 0;
		}
	}


	//TESTS only from here on...
	void Engine::testScripts() {
		//test a script object...
		std::string script_file1 = "script1";
		std::string script_file2 = "script2";

		std::shared_ptr<Script> script_1 = std::make_shared<Script>();
		script_1->setScript(script_file1);
		std::shared_ptr<Script> script_2 = std::make_shared<Script>();
		script_2->setScript(script_file2);

		//add them to component management
		mComponentManagement->addComponent(script_1);
		mComponentManagement->addComponent(script_2);

		std::chrono::time_point<std::chrono::high_resolution_clock> time_start, time_end;
		
		time_start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < 3; i++) {
			std::shared_ptr<Script> script = std::make_shared<Script>();
			script->setScript(script_file1);
			mComponentManagement->addComponent(script);
		}
		time_end = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> elapsed_time = time_end - time_start;
		
		std::cout << "allocs duration = " << elapsed_time.count() << std::endl;
	}

};