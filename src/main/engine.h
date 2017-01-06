#ifndef ENGINE_H
#define ENGINE_H

class Engine
{
public:
	Engine();

	bool isDebug();
	bool isReady();

private:
	bool debug;
	bool ready;

	bool init_stage_1_stuff();
};

#endif // ENGINE_H