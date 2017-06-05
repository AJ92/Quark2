#ifndef ENGINE_H
#define ENGINE_H

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include "vulkan.h"

#include <memory>
//#include <audio.h>


class Engine
{
public:
	Engine();
	~Engine();

	void run();

private:
	bool _debug;

	bool _pre_init();
	bool _init_window();
	bool _init_audio();

	//VULKAN INIT
	bool _init_vulkan();


	bool _post_init();
	bool _main_loop();


	GLFWwindow * _window;

	int _window_width;
	int _window_height;

    //std::shared_ptr<Audio> _audio;
	std::shared_ptr<Vulkan> _vulkan;

};

#endif // ENGINE_H
