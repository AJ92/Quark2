#include "entitymanagement.h"
#include <iostream>


EntityManagement::EntityManagement() {
	bool pre_init_done = pre_init();
	bool post_init_done = post_init();

	if (pre_init_done &&
		post_init_done) {
		std::cout << "EntityManagement initialized" << std::endl;
	}
	else {
		std::cout << "EntityManagement initialization failed" << std::endl;
	}
}

EntityManagement::~EntityManagement() {
	bool pre_deinit_done = pre_deinit();
	bool post_deinit_done = post_deinit();

	if (pre_deinit_done &&
		post_deinit_done) {
		std::cout << "EntityManagement deinitialized" << std::endl;
	}
	else {
		std::cout << "EntityManagement deinitialization failed" << std::endl;
	}
}






///////////////////////////////////////////////
//
//		PRIVATE

bool EntityManagement::pre_init() {

	return true;
}

bool EntityManagement::post_init() {

	return true;
}

bool EntityManagement::pre_deinit() {
	return true;
}

bool EntityManagement::post_deinit() {
	return true;
}
