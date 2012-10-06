/*
 * server.cpp
 *
 *  Created on: Oct 6, 2012
 *      Author: Uditha Atukorala
 */

#include <iostream>
#include <stdlib.h>
#include <pthread.h>

using namespace std;

#include <socket.h>
using namespace socketlibrary;


void * worker( void *arg );      // worket thread


int main(int argc, char *argv[]) {

	pthread_t client;
	pthread_attr_t pattr;

	TCPSocket * client_sock;

	try {

		TCPServerSocket server_sock( 18080 );

		/* set thread create attributes */
		pthread_attr_init(&pattr);
		pthread_attr_setdetachstate(&pattr, PTHREAD_CREATE_DETACHED);

		for (;;) {

			cout << "Waiting for a new connection ...." << endl;

			client_sock = server_sock.accept();
			cout << "New connection accepted on " << client_sock->getForeignAddress() << ":" << client_sock->getForeignPort() << endl;

			pthread_create( &client, &pattr, worker, (void *)client_sock );
		}

	} catch( SocketException &e ) {
		cout << e.what() << endl;
	}
}

// worker thread
void * worker( void *arg ) {

	TCPSocket * cl_sock = ( TCPSocket * ) arg;

	std::string cl_addr = "";
	unsigned short cl_port = 0;

	try {
		cl_addr = cl_sock->getForeignAddress();
		cl_port = cl_sock->getForeignPort();

	} catch ( SocketException &e ) {
		cout << e.what() << endl;
	}

	cout << "client closed " << cl_sock->getForeignAddress() << ":" << cl_sock->getForeignPort() << endl;

	pthread_exit(0);          /* terminate the thread */

}
