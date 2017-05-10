#include "entity.h"
#include <iostream>


Entity::Entity() {
	bool pre_init_done = pre_init();
	bool post_init_done = post_init();

	if (pre_init_done &&
		post_init_done) {
		std::cout << "Entity initialized" << std::endl;
	}
	else {
		std::cout << "Entity initialization failed" << std::endl;
	}
}

Entity::~Entity() {
	bool pre_deinit_done = pre_deinit();
	bool post_deinit_done = post_deinit();

	if (pre_deinit_done &&
		post_deinit_done) {
		std::cout << "Entity deinitialized" << std::endl;
	}
	else {
		std::cout << "Entity deinitialization failed" << std::endl;
	}
}






///////////////////////////////////////////////
//
//		PRIVATE

bool Entity::pre_init() {

	return true;
}

bool Entity::post_init() {

	return true;
}

bool Entity::pre_deinit() {
	return true;
}

bool Entity::post_deinit() {
	return true;
}
