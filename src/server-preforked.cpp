/*
 * server-preforked.cpp
 *
 *  Created on: Oct 7, 2012
 *      Author: Uditha Atukorala
 */

#include <iostream>
#include <socket.h>
#include <unistd.h>

#define PORT "4040"
#define NUM_CHILDREN 5

using namespace std;
using namespace socketlibrary;

int main() {

	int i = 0;
	pid_t pid;

	for ( i = 0; i < NUM_CHILDREN; i++ ) {

		pid = fork();
		if ( pid == -1 ) {
			cout << "ERROR: couldn't fork" << endl;
			return -1;
		}

		if ( pid == 0 ) {
			// we are inside a child
			cout << "child " << i << ", pid: " << pid << endl;

			for (;;) {
				// wait and accept connections
				// read and echo...
				// exit after closing the connection
			}
		}

	}

	// wait for all child processes to exit
	while ( waitpid( -1, NULL, 0 ) > 0 ) {
		// waiting
	}

	return 0;

}

