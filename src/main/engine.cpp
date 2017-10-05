#include "engine.h"
#include <iostream>

Engine::Engine() :
	_window(0)
{

}

Engine::~Engine() {

}

void Engine::run() {
	_pre_init();
	_init_allcoators();
	_init_component_management();
	_init_window();
	_init_audio();
	_init_scripting();
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

bool Engine::_init_allcoators() {
	return true;
}

bool Engine::_init_component_management() {
	_component_management = std::make_shared<ComponentManagement>();
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

bool Engine::_init_scripting() {
	_script_system = std::make_shared<ScriptSystem>(_component_management);
	return true;
}

//AUDIO TEST
bool Engine::_post_init() {
	std::string filename2 = "C:/Code/VS/engine-main/engine/build/Debug/test3.flac";
	//_audio->processEvent(AudioEventType::start, &_audio->getSound(filename2), true);

	//test_p_math();
	//test_allocs();
	test_scripts();

	return true;
}

bool Engine::_main_loop() {
	_script_system->init();
	_init_diagnostics();

	while (!glfwWindowShouldClose(_window)) {
		glfwPollEvents();
		_script_system->update();
		_vulkan->updateUniformBuffer();
		_vulkan->drawFrame();

		_update_diagnostics();
	}
	_vulkan->cleanUp();
	glfwDestroyWindow(_window);
	glfwTerminate();
	return true;
}



void Engine::_init_diagnostics() {
	_current_frame = 0;
	_cum_frame_count = 0;
	_cum_frame_time_elapsed = std::chrono::duration<double>(0.0);
	_frame_start = std::chrono::high_resolution_clock::now();
}

void Engine::_update_diagnostics() {
	_frame_end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed_seconds = _frame_end - _frame_start;
	_cum_frame_time_elapsed += elapsed_seconds;
	_frame_start = _frame_end;

	_cum_frame_count += 1;

	if (_cum_frame_time_elapsed.count() > 1.0) {
		//print fps

		std::cout << "mspf = " << _cum_frame_time_elapsed.count() / _cum_frame_count <<
		"  fps = " << 1.0 / _cum_frame_time_elapsed.count() * _cum_frame_count << std::endl;

		_cum_frame_time_elapsed = std::chrono::duration<double>(0.0);
		_cum_frame_count = 0;
	}
}







//TESTS only from here on...
void Engine::test_scripts() {
	//test a script object...
	std::string script_file1 = "resources/scripts/script1.py";
	std::string script_file2 = "resources/scripts/script2.py";

	std::shared_ptr<Script> script_1 = std::make_shared<Script>();
	script_1->setScript(script_file1);
	std::shared_ptr<Script> script_2 = std::make_shared<Script>();
	script_2->setScript(script_file2);

	//add them to component management
	_component_management->addComponent(script_1);
	_component_management->addComponent(script_2);






	for (int i = 0; i < 3; i++) {
		Script * s = new Script();
		//_component_management->addComponent(std::shared_ptr<Script>(s));
		delete s;
	}

	std::chrono::time_point<std::chrono::high_resolution_clock> time_start, time_end;
	
	time_start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 3; i++) {
		Script * s = new Script();
		s->setScript(script_file1);
		_component_management->addComponent(std::shared_ptr<Script>(s));
	}
	time_end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> elapsed_time = time_end - time_start;
	
	std::cout << "allocs duration = " << elapsed_time.count() << std::endl;
}