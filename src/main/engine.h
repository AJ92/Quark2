#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include <audio.h>


class Engine
{
public:
	Engine();
	~Engine();

	bool isDebug();
	bool isReady();

	std::shared_ptr<Audio> getAudio();

private:
	bool debug;
	bool ready;

	bool pre_init();
	bool init_graphics();
	bool init_physics();
	bool init_audio();
	bool post_init();

	bool pre_deinit();
	bool deinit_graphics();
	bool deinit_physics();
	bool deinit_audio();
	bool post_deinit();

    std::shared_ptr<Audio> audio;
};

#endif // ENGINE_H