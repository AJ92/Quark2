#ifndef SYSTEM_H
#define SYSTEM_H

class System
{
public:
	System();
	~System();

	virtual void update() = 0;
};

#endif // SYSTEM_H