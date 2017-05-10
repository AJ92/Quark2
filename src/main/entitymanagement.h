#ifndef ENTITYMANAGEMENT_H
#define ENTITYMANAGEMENT_H

#include <memory>
#include <map>

#include "entity.h"

class EntityManagement
{
public:
	EntityManagement();
	~EntityManagement();

private:
	bool pre_init();
	bool post_init();

	bool pre_deinit();
	bool post_deinit();

	std::map<std::string, std::shared_ptr<Entity> > _id_entity_map;
};

#endif // ENTITYMANAGEMENT_H