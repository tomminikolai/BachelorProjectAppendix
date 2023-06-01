#pragma once
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class Sock_Client
{
public:
	Sock_Client(const char* addr, const char* port);
	~Sock_Client();
	void write(char* buf, int length);
	void read(char* buf, int length);

private:
	WSADATA wsaData;
	SOCKET ConSock;
	struct addrinfo* result, * ptr, hints;
	int iResult;
};