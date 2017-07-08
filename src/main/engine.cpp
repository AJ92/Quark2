#include "engine.h"
#include <iostream>

const std::vector<const char*> validationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};

Engine::Engine() :
	_window(0)
{

}

Engine::~Engine() {

}

void Engine::run() {
	_pre_init();
	_init_window();
	_init_audio();
	_init_python_scripting();
	_init_vulkan();
	_post_init();
	_main_loop();
}

std::shared_ptr<Vulkan> Engine::getVulkanRenderer() {
	return _vulkan;
}


///////////////////////////////////////////////
//
//		PRIVATE

bool Engine::_pre_init() {
#ifndef _DEBUG
#	if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	//FreeConsole();
#	endif
	std::cout << "RELEASE" << std::endl;
	_debug = false;
#else
	std::cout << "DEBUG" << std::endl;
	_debug = true;
#endif

	return true;
}

bool Engine::_init_window() {
	glfwInit();

	_window_width = 800;
	_window_height = 600;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	_window = glfwCreateWindow(_window_width, _window_height, "Quark2", nullptr, nullptr);
	glfwSetWindowUserPointer(_window, this);
	glfwSetWindowSizeCallback(_window, Engine::_on_window_resized);

	return true;
}

bool Engine::_init_vulkan() {
	_vulkan = std::make_shared<Vulkan>(_window, _window_width, _window_height, _debug);
	return true;
}

void Engine::_on_window_resized(GLFWwindow* window, int width, int height) {
	if (width == 0 || height == 0) return;

	Engine* engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
	engine->getVulkanRenderer()->windowResize(width, height);
}

bool Engine::_init_audio() {
	//_audio = std::make_shared<Audio>(_debug);
	return true;
}

bool Engine::_init_python_scripting() {
	_python_scripting = std::make_shared<ScriptSystem>();
	return true;
}

//AUDIO TEST
bool Engine::_post_init() {
	std::string filename2 = "C:/Code/VS/engine-main/engine/build/Debug/test3.flac";
	//_audio->processEvent(AudioEventType::start, &_audio->getSound(filename2), true);
	return true;
}

bool Engine::_main_loop() {
	while (!glfwWindowShouldClose(_window)) {
		glfwPollEvents();
		_vulkan->updateUniformBuffer();
		_vulkan->drawFrame();
	}
	_vulkan->cleanUp();
	glfwDestroyWindow(_window);
	glfwTerminate();
	return true;
}
