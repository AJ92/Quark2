#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <memory>
#include <map>

class Entity
{
public:
	Entity(std::string name);
	Entity();
	~Entity();

	std::string getIdentifier();

private:

	std::string _identifier;
};

#endif // ENTITY_H