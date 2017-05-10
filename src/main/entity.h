#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include <map>

class Entity
{
public:
	Entity();
	~Entity();

private:
	bool pre_init();
	bool post_init();

	bool pre_deinit();
	bool post_deinit();
};

#endif // ENTITYMANAGEMENT_H