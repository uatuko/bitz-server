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

#include <securesocket.h>
#include <openssl/err.h>			// For OpenSSL errors

#ifndef WIN32
	#include <sys/types.h>			// For data types
	#include <sys/socket.h>			// For socket(), connect(), send(), and recv()
	#include <netdb.h>				// For gethostbyname()
	#include <arpa/inet.h>			// For inet_addr()
	#include <unistd.h>				// For close()
#endif


namespace socketlibrary {

SecureSocket::SecureSocket(int type, int protocol) throw (SocketException) : Socket(type, protocol) {	
	initialise();
}

SecureSocket::SecureSocket(int new_sock, SSL *n_ssl) : Socket(new_sock) {	
	this->ssl_ctx = NULL;		// Clear TLS/SSL contect
	this->ssl = n_ssl;
}

void SecureSocket::initialise(void) {
	// Initialise the SSL library and load error strings
	SSL_library_init();
	SSL_load_error_strings();

	this->ssl_ctx = NULL;		// Clear TLS/SSL contect
}

SecureSocket::~SecureSocket() {
	ERR_free_strings();
	SSL_free(ssl);
	
	if (ssl_ctx != NULL)
		SSL_CTX_free(ssl_ctx);
}


// SecureCommunicatingSocket Code

SecureCommunicatingSocket::SecureCommunicatingSocket(int type, int protocol) throw (SocketException) : SecureSocket(type, protocol) {
}

SecureCommunicatingSocket::SecureCommunicatingSocket(int new_sock, SSL *n_ssl) : SecureSocket(new_sock, n_ssl) {
}

void SecureCommunicatingSocket::connect(const string &foreignAddress, unsigned short foreignPort) throw(SocketException) {
	// Get the address of the requested host
	sockaddr_in destAddr;
	fillAddr(foreignAddress, foreignPort, destAddr);

	// Try to connect to the given port
	if (::connect(sock, (sockaddr *) &destAddr, sizeof(destAddr)) < 0) {
		throw SocketException("Connect failed (connect())", true);
	}
	
	initialise_ctx();
	
	SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_NONE, NULL);		// ??
	ssl = SSL_new(ssl_ctx);
	
	SSL_set_fd(ssl, sock);
	
	if (SSL_connect(ssl) != 1) {
		throw SocketException("Failed to complete TLS/SSL handshake (SSL_connect())", true);
	}
}

void SecureCommunicatingSocket::send(const void *buffer, int bufferLen) throw(SocketException) {
	if (SSL_write(ssl,(raw_type *) buffer, bufferLen) < 0) {
		throw SocketException("Send failed (SSL_write())", true);
	}
}

int SecureCommunicatingSocket::recv(void *buffer, int bufferLen) throw(SocketException) {
	int rtn;
	if ((rtn = SSL_read(ssl,(raw_type *) buffer, bufferLen)) < 0) {
		throw SocketException("Receive failed (SSL_read())");
	}
	return rtn;
}

bool SecureCommunicatingSocket::waiting_R(unsigned int _timeout) throw (SocketException) {

	fd_set read_fds;
	struct timeval tv;

	FD_ZERO(&read_fds);
	FD_SET(sock, &read_fds);

	tv.tv_sec = 0;
	tv.tv_usec = _timeout * 1000;

	if (select(sock + 1, &read_fds, NULL, NULL, &tv) == -1) {
		throw SocketException("Failed to select read fds (select())", true);
	}

	if (FD_ISSET(sock, &read_fds)) {
		return true;
	} else {
		return false;
	}

}

string SecureCommunicatingSocket::getForeignAddress() throw (SocketException) {
	sockaddr_in addr;
	unsigned int addr_len = sizeof(addr);

	if (getpeername(sock, (sockaddr *) &addr,(socklen_t *) &addr_len) < 0) {
		throw SocketException("Fetch of foreign address failed (getpeername())", true);
	}
	return inet_ntoa(addr.sin_addr);
}

unsigned short SecureCommunicatingSocket::getForeignPort() throw (SocketException) {
	sockaddr_in addr;
	unsigned int addr_len = sizeof(addr);

	if (getpeername(sock, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0) {
		throw SocketException("Fetch of foreign port failed (getpeername())", true);
	}
	return ntohs(addr.sin_port);
}

void SecureCommunicatingSocket::initialise_ctx(void) throw (SocketException) {

	const SSL_METHOD *ssl_method;						// The method describes which SSL protocol we will be using.
	
	ssl_method = SSLv23_client_method();		// Compatible with SSLv2, SSLv3 and TLSv1
	ssl_ctx = SSL_CTX_new(ssl_method);			// Create new context from method
	
	if(ssl_ctx == NULL) {
		throw SocketException("Failed to create new context", true);
	}
	
}


// SecureTCPSocket Code

SecureTCPSocket::SecureTCPSocket() throw (SocketException) : SecureCommunicatingSocket(SOCK_STREAM, IPPROTO_TCP) {
}

SecureTCPSocket::SecureTCPSocket(int new_sock, SSL *n_ssl) : SecureCommunicatingSocket(new_sock, n_ssl) {
}


// SecureTCPServerSocket code

SecureTCPServerSocket::SecureTCPServerSocket(unsigned short localPort, const char *cert_file, const char *key_file, int queueLen) throw (SocketException) : SecureSocket(SOCK_STREAM, IPPROTO_TCP) {
	setLocalPort(localPort);
	setListen(cert_file, key_file, queueLen);
}

SecureTCPServerSocket::SecureTCPServerSocket(const string &localAddress, unsigned short localPort, const char *cert_file, const char *key_file, int queueLen) throw (SocketException) : SecureSocket(SOCK_STREAM, IPPROTO_TCP) {
	setLocalAddressAndPort(localAddress, localPort);
	setListen(cert_file, key_file, queueLen);
}

SecureTCPSocket *SecureTCPServerSocket::accept() throw (SocketException) {
	int cl_sock;	
	if ((cl_sock = ::accept(sock, NULL, 0)) < 0) {
		throw SocketException("Accept failed (accept())", true);
	}
	
	ssl = SSL_new(ssl_ctx); 
	SSL_set_fd(ssl, cl_sock);
	
	if (SSL_accept(ssl) != 1) {
		throw SocketException("Failed to complete TLS/SSL handshake (SSL_accept())");
	}
	
	return new SecureTCPSocket(cl_sock, ssl);
}

void SecureTCPServerSocket::initialise_ctx(void) throw (SocketException) {

	const SSL_METHOD *ssl_method;						// The method describes which SSL protocol we will be using.

	ssl_method = SSLv23_server_method();		// Compatible with SSLv2, SSLv3 and TLSv1
	ssl_ctx = SSL_CTX_new(ssl_method);			// Create new context from method

	if(ssl_ctx == NULL) {
		throw SocketException("Failed to create new TLS/SSL context");
	}

	if (SSL_CTX_use_certificate_chain_file(ssl_ctx, this->cert) <= 0) {
		throw SocketException("Failed to load certificate file SSL_CTX_use_certificate_chain_file()");
	}

	if (SSL_CTX_use_PrivateKey_file(ssl_ctx, this->key, SSL_FILETYPE_PEM) <= 0) {
		throw SocketException("Failed to load private key file SSL_CTX_use_PrivateKey_file()");
	}

	// Verify that the two keys goto together.
	if (!SSL_CTX_check_private_key(ssl_ctx)) {
		throw SocketException("Private key does not match the certificate public key");
	}

}

void SecureTCPServerSocket::setListen(const char *cert_file, const char *key_file, int queueLen) throw (SocketException) {

	this->cert = cert_file;
	this->key = key_file;
	
	if (listen(sock, queueLen) < 0) {
		throw SocketException("Set listening socket failed (listen())", true);
	}
	
	initialise_ctx();
}


} // End of namespace SocketLibrary
