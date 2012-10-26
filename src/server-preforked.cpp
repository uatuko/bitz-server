/*
 * server-preforked.cpp
 *
 *  Created on: Oct 7, 2012
 *      Author: Uditha Atukorala
 */

#include <iostream>
#include <socket.h>
#include <unistd.h>
#include <sys/wait.h>

#define PORT 4040
#define NUM_CHILDREN 5
#define MAXLEN 1024

using namespace std;
using namespace socketlibrary;

int main() {

	int i = 0;
	pid_t pid;

	TCPSocket * client_sock;
	TCPServerSocket * server_sock;

	server_sock = new TCPServerSocket( PORT );

	for ( i = 0; i < NUM_CHILDREN; i++ ) {

		pid = fork();
		if ( pid == -1 ) {
			cout << "ERROR: couldn't fork" << endl;
			return -1;
		}

		if ( pid == 0 ) {

			// we are inside a child
			cout << "child " << i << ", pid: " << pid << endl;

			int  line_len;
			char line[MAXLEN];

			for (;;) {

				cout << "Waiting for a new connection [" << i << "]..." << endl;

				try {

					client_sock = server_sock->accept();
					cout << "[" << i << "] New connection accepted on " << client_sock->getForeignAddress() << ":" << client_sock->getForeignPort() << endl;

					// read
					line_len = client_sock->readLine( line, MAXLEN );
					if ( line_len == -1) {
						cout << "[" << i << "] Failed to read from connection" << endl;
					} else {

						cout << "[" << i << "] client said: " << line << endl;

						// echo back
						client_sock->send( line, line_len );

					}

					// destroy / close connection
					delete client_sock;

				} catch( SocketException &e ) {
					cout << "[" << i << "] ERROR: " << e.what() << endl;
				}
			}

		}

	}

	// wait for all child processes to exit
	while ( waitpid( -1, NULL, 0 ) > 0 ) {
		// waiting
	}

	return 0;

}

