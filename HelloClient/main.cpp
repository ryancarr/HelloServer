#define _WINSOCK_DEPRECATED_NO_WARNINGS

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
#include <string.h>
using namespace std;

int main()
{
	StartSocketLib;

	// Create a connectiong data socket
	int err;
	char message[128] = "Hello Internet!";
	char ip[16] = "";
	unsigned long ipaddr;

	cout << "Enter the IP address to connect to: ";
	cin >> ip;

	// Convert the IP address
	ipaddr = inet_addr(ip);
	if (ipaddr == -1)
	{
		cout << "Error: invalid IP address" << endl;
		return 0;
	}

	// Create a socket
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Create a sockaddr_in for connection on port 4000
	struct sockaddr_in socketaddress;
	socklen_t sa_size = sizeof(struct sockaddr_in);
	socketaddress.sin_family = AF_INET;
	socketaddress.sin_port = htons(4000);
	socketaddress.sin_addr.s_addr = ipaddr;
	memset(&(socketaddress.sin_zero), 0, 8);

	// Connect the socket
	err = connect(sock, (struct sockaddr*)&socketaddress, sa_size);

	// Send message
	err = send(sock, message, strlen(message) + 1, 0);

	shutdown(sock, 2);
	CloseSocket(sock);

	CloseSocketLib;
}