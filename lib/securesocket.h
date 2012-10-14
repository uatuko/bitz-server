/*
 *	C++ sockets on Unix and Windows
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
 
#ifndef SL_SECURE_SOCKET_H
#define SL_SECURE_SOCKET_H

#include "socket.h"
#include <openssl/ssl.h>			// For OpenSSL

namespace socketlibrary {

/**
 *   Base class representing basic communication endpoint
 */
class SecureSocket: public Socket {
public:
	/**
	*   Close and deallocate this socket
	*/
	~SecureSocket();

private:
	void initialise(void);
	
protected:
	SSL_CTX *ssl_ctx;
	SSL *ssl;
	
	SecureSocket(int type, int protocol) throw (SocketException);
	SecureSocket(int new_sock, SSL *n_ssl);
	virtual void initialise_ctx(void) throw (SocketException) = 0;
};

/**
 *   TLS/SSL Socket which is able to connect, send, and receive
 */
class SecureCommunicatingSocket: public SecureSocket {
public:
	/**
	*   Establish a socket connection with the given foreign
	*   address and port using TLS/SSL
	*   @param foreignAddress foreign address (IP address or name)
	*   @param foreignPort foreign port
	*   @exception SocketException thrown if unable to establish connection
	*/
	void connect(const string &foreignAddress, unsigned short foreignPort) throw (SocketException);
	
	/**
	*   Write the given buffer to this socket.  Call connect() before
	*   calling send()
	*   @param buffer buffer to be written
	*   @param bufferLen number of bytes from buffer to be written
	*   @exception SocketException thrown if unable to send data
	*/
	void send(const void *buffer, int bufferLen) throw (SocketException);

	/**
	*   Read into the given buffer up to bufferLen bytes data from this
	*   socket.  Call connect() before calling recv()
	*   @param buffer buffer to receive the data
	*   @param bufferLen maximum number of bytes to read into buffer
	*   @return number of bytes read, 0 for EOF, and -1 for error
	*   @exception SocketException thrown if unable to receive data
	*/
	int recv(void *buffer, int bufferLen) throw (SocketException);
	
	/**
	*	Checks for any incoming data
	*	@param optional timeout in milliseconds (default 1ms)
	*	@return boolean
	*	@exception SocketException thrown on error
	*/
	bool waiting_R(const unsigned int _timeout = 1) throw (SocketException);

	/**
	*   Get the foreign address.  Call connect() before calling recv()
	*   @return foreign address
	*   @exception SocketException thrown if unable to fetch foreign address
	*/
	string getForeignAddress() throw (SocketException);

	/**
	*   Get the foreign port.  Call connect() before calling recv()
	*   @return foreign port
	*   @exception SocketException thrown if unable to fetch foreign port
	*/
	unsigned short getForeignPort() throw (SocketException);
	
protected:
	SecureCommunicatingSocket(int type, int protocol) throw (SocketException);
	SecureCommunicatingSocket(int new_sock, SSL *n_ssl);
	void initialise_ctx(void) throw (SocketException);
};


/**
 *   TLS/SSL TCP socket for communicating with other TLS/SSL TCP sockets
 */
class SecureTCPSocket: public SecureCommunicatingSocket {
public:
	/**
	*   Construct a Secure TCP socket with no connection
	*   @exception SocketException thrown if unable to create TCP socket
	*/
	SecureTCPSocket() throw (SocketException);
private:
	friend class SecureTCPServerSocket;
	SecureTCPSocket(int new_sock, SSL *n_ssl);
};

/**
 *   Secure TCP socket class for TLS/SSL servers
 */
class SecureTCPServerSocket: public SecureSocket {
public:
	/**
	*   Construct a Secure TCP socket for use with a server, accepting connections
	*   on the specified port on any interface
	*   @param localPort local port of server socket, a value of zero will
	*                   give a system-assigned unused port
	*   @param cert_file certificate file to use with TLS/SSL connections
	*   @param key_file private key file for the TLS/SSL certificate
	*   @param queueLen maximum queue length for outstanding 
	*                   connection requests (default 5)
	*   @exception SocketException thrown if unable to create TCP server socket
	*/
	SecureTCPServerSocket(unsigned short localPort, const char *cert_file, const char *key_file, int queueLen = 5) throw (SocketException);
	
	/**
	*   Construct a TCP socket for use with a server, accepting connections
	*   on the specified port on the interface specified by the given address
	*   @param localAddress local interface (address) of server socket
	*   @param localPort local port of server socket
	*   @param cert_file certificate file to use with TLS/SSL connections
	*   @param key_file private key file for the TLS/SSL certificate
	*   @param queueLen maximum queue length for outstanding 
	*                   connection requests (default 5)
	*   @exception SocketException thrown if unable to create TCP server socket
	*/
	SecureTCPServerSocket(const string &localAddress, unsigned short localPort, const char *cert_file, const char *key_file, int queueLen = 5) throw (SocketException);
	
	/**
	*   Blocks until a new connection is established on this socket or error
	*   @return new secure connection socket
	*   @exception SocketException thrown if attempt to accept a new connection fails
	*/
	SecureTCPSocket *accept() throw (SocketException);
	
private:
	const char *cert;		// certificate file
	const char *key;		// private key file
	
	void initialise_ctx(void) throw (SocketException);
	void setListen(const char *cert_file, const char *key_file, int queueLen) throw (SocketException);
};

} // End of namespace SocketLibrary

#endif /* !SL_SECURE_SOCKET_H */
