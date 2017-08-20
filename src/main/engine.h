#ifndef ENGINE_H
#define ENGINE_H

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include "base/vulkan/vulkan.h"
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
	bool _debug;

	bool _pre_init();
	bool _init_allcoators();
	bool _init_component_management();
	bool _init_window();
	bool _init_audio();
	bool _init_scripting();

	//VULKAN INIT
	bool _init_vulkan();
	static void _on_window_resized(GLFWwindow* window, int width, int height);

	bool _post_init();
	bool _main_loop();


	GLFWwindow * _window;

	int _window_width;
	int _window_height;

	std::shared_ptr<ComponentManagement> _component_management;

    //std::shared_ptr<Audio> _audio;
	std::shared_ptr<Vulkan> _vulkan;
	std::shared_ptr<ScriptSystem> _script_system;



	//diagnostics... probably better to put it in an extra system!
	void _init_diagnostics();
	void _update_diagnostics();

	std::chrono::time_point<std::chrono::high_resolution_clock> _frame_start, _frame_end;
	std::chrono::duration<double> _cum_frame_time_elapsed;
	int _cum_frame_count;
	int _max_frame_count;
	int _current_frame;



	//tests
	void test_scripts();
	void test_p_math();
	void test_allocs();
};

#endif // ENGINE_H
