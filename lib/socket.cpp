/*
 *	C++ sockets on Unix and Windows
 *	Copyright (C) 2002	<unknown>
 *	Copyright (C) 2010	Uditha Atukorala
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 
#include <socket.h>

#ifndef WIN32
	#include <sys/types.h>			// For data types
	#include <sys/socket.h>			// For socket(), connect(), send(), and recv()
	#include <netdb.h>				// For gethostbyname()
	#include <arpa/inet.h>			// For inet_addr()
	#include <unistd.h>				// For close()
#endif

#include <cerrno>					// For errno
#include <cstring>					// For strerror
#include <cstdlib>					// For atoi()

using namespace std;

namespace socketlibrary {

#ifdef WIN32
	static bool initialized = false;
#endif

// SocketException Code

SocketException::SocketException(const string &message, bool inclSysMsg) throw() : userMessage(message) {
	if (inclSysMsg) {
		userMessage.append(": ");
		userMessage.append(strerror(errno));
	}
}

SocketException::~SocketException() throw() {
}

const char *SocketException::what() const throw() {
	return userMessage.c_str();
}

// Function to fill in address structure given an address and port
void fillAddr(const string &address, unsigned short port, sockaddr_in &addr) {

	memset(&addr, 0, sizeof(addr));  // Zero out address structure
	addr.sin_family = AF_INET;       // Internet address

	hostent *host;  // Resolve name
	if ((host = gethostbyname(address.c_str())) == NULL) {
		// strerror() will not work for gethostbyname() and hstrerror() 
		// is supposedly obsolete
		throw SocketException("Failed to resolve name (gethostbyname())");
	}
	
	addr.sin_addr.s_addr = *((unsigned long *) host->h_addr_list[0]);
	addr.sin_port = htons(port);     // Assign port in network byte order
	
}

// Socket Code

Socket::Socket(int type, int protocol) throw(SocketException) {
#ifdef WIN32
	if (!initialized) {
		WORD wVersionRequested;
		WSADATA wsaData;

		wVersionRequested = MAKEWORD(2, 0);							// Request WinSock v2.0
		if (WSAStartup(wVersionRequested, &wsaData) != 0) {			// Load WinSock DLL
			throw SocketException("Unable to load WinSock DLL");
		}
		initialized = true;
	}
#endif

	// Make a new socket
	if ((sock = socket(PF_INET, type, protocol)) < 0) {
		throw SocketException("Socket creation failed (socket())", true);
	}
}

Socket::Socket(int sock) {
	this->sock = sock;
}

Socket::~Socket() {

#ifdef WIN32
	::closesocket(sock);
#else
	::close(sock);
#endif
	sock = -1;
}

string Socket::getLocalAddress() throw(SocketException) {
	sockaddr_in addr;
	unsigned int addr_len = sizeof(addr);

	if (getsockname(sock, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0) {
		throw SocketException("Fetch of local address failed (getsockname())", true);
	}
	return inet_ntoa(addr.sin_addr);
}

unsigned short Socket::getLocalPort() throw(SocketException) {
	sockaddr_in addr;
	unsigned int addr_len = sizeof(addr);

	if (getsockname(sock, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0) {
		throw SocketException("Fetch of local port failed (getsockname())", true);
	}
	return ntohs(addr.sin_port);
}

void Socket::setLocalPort(unsigned short localPort) throw(SocketException) {
	// Bind the socket to its port
	sockaddr_in localAddr;
	memset(&localAddr, 0, sizeof(localAddr));
	localAddr.sin_family = AF_INET;
	localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	localAddr.sin_port = htons(localPort);

	if (bind(sock, (sockaddr *) &localAddr, sizeof(sockaddr_in)) < 0) {
		throw SocketException("Set of local port failed (bind())", true);
	}
}

void Socket::setLocalAddressAndPort(const string &localAddress, unsigned short localPort) throw(SocketException) {
	// Get the address of the requested host
	sockaddr_in localAddr;
	fillAddr(localAddress, localPort, localAddr);

	if (bind(sock, (sockaddr *) &localAddr, sizeof(sockaddr_in)) < 0) {
		throw SocketException("Set of local address and port failed (bind())", true);
	}
}

void Socket::cleanUp() throw(SocketException) {
#ifdef WIN32
	if (WSACleanup() != 0) {
		throw SocketException("WSACleanup() failed");
	}
#endif
}

unsigned short Socket::resolveService(const string &service, const string &protocol) {

	struct servent *serv;        /* Structure containing service information */

	if ((serv = getservbyname(service.c_str(), protocol.c_str())) == NULL)
		return atoi(service.c_str());  /* Service is port number */
	else 
		return ntohs(serv->s_port);    /* Found port (network byte order) by name */
	
}

// CommunicatingSocket Code

CommunicatingSocket::CommunicatingSocket(int type, int protocol) throw(SocketException) : Socket(type, protocol) {
}

CommunicatingSocket::CommunicatingSocket(int newSD) : Socket(newSD) {
}

void CommunicatingSocket::connect(const string &foreignAddress, unsigned short foreignPort) throw(SocketException) {
	// Get the address of the requested host
	sockaddr_in destAddr;
	fillAddr(foreignAddress, foreignPort, destAddr);

	// Try to connect to the given port
	if (::connect(sock, (sockaddr *) &destAddr, sizeof(destAddr)) < 0) {
		throw SocketException("Connect failed (connect())", true);
	}
}

void CommunicatingSocket::send(const void *buffer, int bufferLen) throw(SocketException) {
	if (::send(sock, (raw_type *) buffer, bufferLen, 0) < 0) {
		throw SocketException("Send failed (send())", true);
	}
}

int CommunicatingSocket::recv(void *buffer, int bufferLen) throw(SocketException) {
	int rtn;
	if ((rtn = ::recv(sock, (raw_type *) buffer, bufferLen, 0)) < 0) {
		throw SocketException("Received failed (recv()) for socket ", true);
	}
	return rtn;
}

int CommunicatingSocket::peek(void *buffer, int bufferLen) throw(SocketException) {
	int rtn;
	if ((rtn = ::recv(sock, (raw_type *) buffer, bufferLen, MSG_PEEK)) < 0) {
		throw SocketException("Received failed (recv()) for socket ", true);
	}
	return rtn;
}

int CommunicatingSocket::readLine(char *buffer, int bufferLen, const char delimiter) throw(SocketException) {

	int n, rc;
	char c;

	for (n = 1; n < bufferLen; n++) {
		if ((rc = recv(&c, 1)) == 1) {
			*buffer++ = c;
			if (c == delimiter) {
				break;
			}
		} else if (rc == 0) {
			if (n == 1) {
				return 0; // EOF, no data read
			} else {
				break; // EOF, read some data
			}
		} else {
			throw SocketException("Failed to read data from socket ", true);
		}
	}

	*buffer = '\0';
	return n;

}

string CommunicatingSocket::getForeignAddress() throw(SocketException) {
	sockaddr_in addr;
	unsigned int addr_len = sizeof(addr);

	if (getpeername(sock, (sockaddr *) &addr,(socklen_t *) &addr_len) < 0) {
		throw SocketException("Fetch of foreign address failed (getpeername())", true);
	}
	return inet_ntoa(addr.sin_addr);
}

unsigned short CommunicatingSocket::getForeignPort() throw(SocketException) {
	sockaddr_in addr;
	unsigned int addr_len = sizeof(addr);

	if (getpeername(sock, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0) {
		throw SocketException("Fetch of foreign port failed (getpeername())", true);
	}
	return ntohs(addr.sin_port);
}

// TCPSocket Code

TCPSocket::TCPSocket() throw(SocketException) : CommunicatingSocket(SOCK_STREAM, IPPROTO_TCP) {
}

TCPSocket::TCPSocket(const string &foreignAddress, unsigned short foreignPort) throw(SocketException) : CommunicatingSocket(SOCK_STREAM, IPPROTO_TCP) {
	connect(foreignAddress, foreignPort);
}

TCPSocket::TCPSocket(int newSD) : CommunicatingSocket(newSD) {
}

// TCPServerSocket Code

TCPServerSocket::TCPServerSocket(unsigned short localPort, int queueLen) throw(SocketException) : Socket(SOCK_STREAM, IPPROTO_TCP) {
	setLocalPort(localPort);
	setListen(queueLen);
}

TCPServerSocket::TCPServerSocket(const string &localAddress, unsigned short localPort, int queueLen) throw(SocketException) : Socket(SOCK_STREAM, IPPROTO_TCP) {
	setLocalAddressAndPort(localAddress, localPort);
	setListen(queueLen);
}

TCPSocket *TCPServerSocket::accept() throw(SocketException) {
	int newSD;	
	if ((newSD = ::accept(sock, NULL, 0)) < 0) {
		throw SocketException("Accept failed (accept())", true);
	}
	return new TCPSocket(newSD);
}

void TCPServerSocket::setListen(int queueLen) throw(SocketException) {
	if (listen(sock, queueLen) < 0) {
		throw SocketException("Set listening socket failed (listen())", true);
	}
}

// TCPServerSocketM Code 

TCPServerSocketM::TCPServerSocketM(unsigned short localPort, int queueLen) throw(SocketException) : TCPServerSocket(localPort, queueLen) {
	
	FD_ZERO(&fds_master);		// clear the master and temp sets

	FD_SET(sock, &fds_master);	// Add ourself to the master set
	fdmax = sock;				// keep track of the biggest file descriptor, only ourself for the moment
	
}

TCPSocket *TCPServerSocketM::accept() throw(SocketException) {
	
	TCPSocket *newClient = TCPServerSocket::accept();
	int newSD = newClient->sock;

	FD_SET(newSD, &fds_master); 	// add to master set
	if (newSD > fdmax) {    		// keep track of the max
		fdmax = newSD;
	}
	
	v_clients.push_back(newClient);	
	return newClient;
	
}

bool TCPServerSocketM::pendingConnections() throw (SocketException) {
	
	fd_set read_fds;
	FD_ZERO(&read_fds);
	
	read_fds = fds_master;
	if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
		throw SocketException("Failed to select read fds (select())", true);
	}
	
	if (FD_ISSET(sock, &read_fds)) {
		return true;
	} else {
		return false;
	}
	
}

int TCPServerSocketM::getWaitingClients(vector<TCPSocket *> &clients) throw (SocketException) {

	fd_set read_fds;
	FD_ZERO(&read_fds);
	
	int cl_count = 0;
	
	read_fds = fds_master;
	if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
		throw SocketException("Failed to select read fds (select())", true);
	}

	// resize the vector so it doesn't contain any old information
	clients.resize(0);
	
	for(unsigned int i=0; i<v_clients.size(); i++) {
		if (FD_ISSET(v_clients.at(i)->sock, &read_fds)) { // we got one!!
			clients.push_back(v_clients.at(i));
			cl_count++;
		}
	}
	
	return cl_count;
}

void TCPServerSocketM::closeClientConnection(TCPSocket * client) {
	
	for(unsigned int i=0; i<v_clients.size(); i++) {
		if (client->sock == v_clients.at(i)->sock) {			
			FD_CLR(client->sock, &fds_master); 
			v_clients.erase(v_clients.begin() + i);
			delete client;
			break;
		}
	}
	
}


// UDPSocket Code

UDPSocket::UDPSocket() throw(SocketException) : CommunicatingSocket(SOCK_DGRAM, IPPROTO_UDP) {
	setBroadcast();
}

UDPSocket::UDPSocket(unsigned short localPort)  throw(SocketException) : CommunicatingSocket(SOCK_DGRAM, IPPROTO_UDP) {
	setLocalPort(localPort);
	setBroadcast();
}

UDPSocket::UDPSocket(const string &localAddress, unsigned short localPort) throw(SocketException) : CommunicatingSocket(SOCK_DGRAM, IPPROTO_UDP) {
	setLocalAddressAndPort(localAddress, localPort);
	setBroadcast();
}

void UDPSocket::setBroadcast() {
	// If this fails, we'll hear about it when we try to send.  This will allow 
	// system that cannot broadcast to continue if they don't plan to broadcast
	int broadcastPermission = 1;
	setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (raw_type *) &broadcastPermission, sizeof(broadcastPermission));
}

void UDPSocket::disconnect() throw(SocketException) {
	sockaddr_in nullAddr;
	memset(&nullAddr, 0, sizeof(nullAddr));
	nullAddr.sin_family = AF_UNSPEC;

	// Try to disconnect
	if (::connect(sock, (sockaddr *) &nullAddr, sizeof(nullAddr)) < 0) {
#ifdef WIN32
		if (errno != WSAEAFNOSUPPORT) {
#else
		if (errno != EAFNOSUPPORT) {
#endif
			throw SocketException("Disconnect failed (connect())", true);
		}
	}
}

void UDPSocket::sendTo(const void *buffer, int bufferLen, const string &foreignAddress, unsigned short foreignPort) throw(SocketException) {
	sockaddr_in destAddr;
	fillAddr(foreignAddress, foreignPort, destAddr);

	// Write out the whole buffer as a single message.
	if (sendto(sock, (raw_type *) buffer, bufferLen, 0, (sockaddr *) &destAddr, sizeof(destAddr)) != bufferLen) {
		throw SocketException("Send failed (sendto())", true);
	}
}

int UDPSocket::recvFrom(void *buffer, int bufferLen, string &sourceAddress, unsigned short &sourcePort) throw(SocketException) {
	sockaddr_in clntAddr;
	socklen_t addrLen = sizeof(clntAddr);
	int rtn;
	if ((rtn = recvfrom(sock, (raw_type *) buffer, bufferLen, 0, (sockaddr *) &clntAddr, (socklen_t *) &addrLen)) < 0) {
		throw SocketException("Receive failed (recvfrom())", true);
	}
	sourceAddress = inet_ntoa(clntAddr.sin_addr);
	sourcePort = ntohs(clntAddr.sin_port);
	return rtn;
}

void UDPSocket::setMulticastTTL(unsigned char multicastTTL) throw(SocketException) {
	if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (raw_type *) &multicastTTL, sizeof(multicastTTL)) < 0) {
		throw SocketException("Multicast TTL set failed (setsockopt())", true);
	}
}

void UDPSocket::joinGroup(const string &multicastGroup) throw(SocketException) {
	struct ip_mreq multicastRequest;

	multicastRequest.imr_multiaddr.s_addr = inet_addr(multicastGroup.c_str());
	multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (raw_type *) &multicastRequest, sizeof(multicastRequest)) < 0) {
		throw SocketException("Multicast group join failed (setsockopt())", true);
	}
}

void UDPSocket::leaveGroup(const string &multicastGroup) throw(SocketException) {
	struct ip_mreq multicastRequest;

	multicastRequest.imr_multiaddr.s_addr = inet_addr(multicastGroup.c_str());
	multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (raw_type *) &multicastRequest, sizeof(multicastRequest)) < 0) {
		throw SocketException("Multicast group leave failed (setsockopt())", true);
	}
}

} // End of namespace SocketLibrary
