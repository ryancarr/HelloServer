// Include files for Windows or *Nix systems
#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif // WIN32

#pragma comment(lib, "Ws2_32.lib")

// Definitions and globals for cross-compatibility
#ifdef WIN32
WSADATA g_wsadata;
#define CloseSocket closesocket
#define GetSocketError WSAGetLastError
#define StartSocketLib WSAStartup( MAKEWORD (2,2), &g_wsadata);
#define CloseSocketLib WSACleanup();
#ifndef socklen_t
typedef int socklen_t;
#endif // socklen_t
#else
#define CloseSocket close
#define GetSocketError errno
#define StartSocketLib { }
#define CloseSocketLib { }
#endif // WIN32

#include <iostream>
using namespace std;

int main()
{
	int err;
	StartSocketLib;

	// Create listening socket on port 4000
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == -1)
	{
		cout << "Socket creation error!" << endl;
		return 0;
	}

	cout << "Socket created!" << endl;

	// Create a sockaddr_in for binding, listening on port 4000
	struct sockaddr_in socketaddress;
	socklen_t sa_size = sizeof(struct sockaddr_in);
	socketaddress.sin_family = AF_INET;
	socketaddress.sin_port = htons(4000);
	socketaddress.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&(socketaddress.sin_zero), 0, 8);

	// Bind the socket
	err = bind(sock, (struct sockaddr*)&socketaddress, sa_size);

	// Listen on the socket
	err = listen(sock, 16);

	// Wait for an incoming connection now
	int datasock;
	datasock = accept(sock, (struct sockaddr*)&socketaddress, &sa_size);

	// Receive data
	char buffer[128];
	err = recv(datasock, buffer, 128, 0);

	cout << "Data received: " << endl;
	cout << buffer << endl;

	// Shutdown and close sockets
	shutdown(datasock, 2);
	CloseSocket(datasock);

	shutdown(sock, 2);
	CloseSocket(sock);

	CloseSocketLib;
}