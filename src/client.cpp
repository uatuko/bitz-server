/*
 * client.cpp
 *
 *  Created on: Oct 6, 2012
 *      Author: Uditha Atukorala
 */

#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>

using namespace std;

#include <socket.h>
using namespace socketlibrary;

int main(int argc, char *argv[]) {

	string usr_input, s;
	int    i, k, p;
	char   *r;

	cout << "Welcome to my awesome client. type a command or help for the list of commands available." << endl;

	while (1) {
		TCPSocket * server_socket;
		server_socket = new TCPSocket();
		getline(cin, usr_input);
		if (usr_input == "help") {
			cout << "the server is still stupid." << endl;
		} else if (usr_input.find("spam") != string::npos) {
			p = usr_input.find("spam");
			s = usr_input.substr(p + 4, usr_input.length() - p);
			stringstream(s) >> k;
			for (i = 0; i < k; i++) {
				try {
					cout << "in for" << endl;
					server_socket->connect("localhost", 4040);
					server_socket->send("spam\n", 5);
					server_socket->recv(r, 1024);
					cout << r << endl;
					delete server_socket;
				} catch( SocketException &e) {
					cout << e.what() << endl;
				}
			}
		} else {
			try {
				server_socket->connect("localhost", 4040);
				server_socket->send("cmd\n", 4);
				server_socket->recv(r, 1024);
				cout << r << endl;

			} catch( SocketException &e) {
				cout << e.what() << endl;
			}
		}
		//delete server_socket;
	}
}
