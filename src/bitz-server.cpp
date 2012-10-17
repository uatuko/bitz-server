/*
 * bitz-server.cpp
 *
 *  Created on: Oct 7, 2012
 *      Author: Uditha Atukorala
 */

#include <config.h>
#include <iostream>

#include "bitz/manager.h"

using namespace bitz;

int main() {

	std::cout << PACKAGE_STRING << std::endl;

	// TODO
	Manager * manager = new Manager( 4040 );

	return 0;
}
