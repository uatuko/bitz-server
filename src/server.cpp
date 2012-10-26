/*
 * server.cpp
 *
 *  Created on: Oct 6, 2012
 *      Author: Uditha Atukorala
 */

#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <socket.h>

#define PORT 4040
#define MAXLEN 1024

using namespace std;
using namespace socketlibrary;

// forward declarations
void * worker( void *arg );      // worket thread

int main(int argc, char *argv[]) {

	pthread_t client;
	pthread_attr_t pattr;

	TCPSocket * client_sock;

	try {

		TCPServerSocket server_sock( PORT );

		// set create attributes for the threads
		pthread_attr_init(&pattr);
		pthread_attr_setdetachstate(&pattr, PTHREAD_CREATE_DETACHED);

		for (;;) {

			cout << "Waiting for a new connection ...." << endl;

			client_sock = server_sock.accept();
			cout << "New connection accepted on " << client_sock->getForeignAddress() << ":" << client_sock->getForeignPort() << endl;

			// serve the client in a new thread
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

	int  line_len;
	char line[MAXLEN];

	try {

		cl_addr = cl_sock->getForeignAddress();
		cl_port = cl_sock->getForeignPort();

		// read from the client
		line_len = cl_sock->readLine( line, MAXLEN );

		cout << "[" << cl_sock->getForeignAddress() << ":" << cl_sock->getForeignPort() << "] client said: " << line << endl;

	} catch ( SocketException &e ) {
		cout << e.what() << endl;
	}

	cout << "client closed " << cl_sock->getForeignAddress() << ":" << cl_sock->getForeignPort() << endl;

	// destroy / close connection
	delete cl_sock;

	// terminate the thread
	pthread_exit(0);

}
