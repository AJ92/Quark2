#ifndef ISYSTEM_H
#define ISYSTEM_H

class ISystem
{
public:
	ISystem();
	virtual ~ISystem() = default;

	virtual void init() = 0;
	virtual void update() = 0;
};

#endif // ISYSTEM_H