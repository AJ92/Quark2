#ifndef ENGINE_H
#define ENGINE_H

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include "vulkan.h"
#include "base\systems\scriptsystem.h"

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
	bool _init_window();
	bool _init_audio();
	bool _init_python_scripting();

	//VULKAN INIT
	bool _init_vulkan();
	static void _on_window_resized(GLFWwindow* window, int width, int height);

	bool _post_init();
	bool _main_loop();


	GLFWwindow * _window;

	int _window_width;
	int _window_height;

    //std::shared_ptr<Audio> _audio;
	std::shared_ptr<Vulkan> _vulkan;
	std::shared_ptr<ScriptSystem> _python_scripting;
};

#endif // ENGINE_H
