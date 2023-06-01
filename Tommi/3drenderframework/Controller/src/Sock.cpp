#include "..\include\Sock.h"

Sock_Client::Sock_Client(const char* addr, const char* port)
	: result(NULL), ptr(NULL)
{
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSA startup failed with code: %d\n", iResult);
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	iResult = getaddrinfo((PCSTR)addr, (PCSTR)port, &hints, &result);
	if (iResult != 0)
	{
		printf("Get address info failed with code: %d\n", iResult);
		WSACleanup();
		return;
	}
	for (ptr = result; ptr != NULL; ptr->ai_next)
	{
		ConSock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConSock == INVALID_SOCKET)
		{
			printf("socket failed with error code: %ld\n", WSAGetLastError());
			WSACleanup();
			return;
		}
		iResult = connect(ConSock, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			closesocket(ConSock);
			ConSock = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConSock == INVALID_SOCKET)
	{
		printf("error connecting to server");
		WSACleanup();
		return;
	}
}

Sock_Client::~Sock_Client()
{
	shutdown(ConSock, SD_SEND);
	closesocket(ConSock);
	WSACleanup();
}

void Sock_Client::write(char* buf, int length)
{
	iResult = send(ConSock, buf, length, 0);
	if (iResult == SOCKET_ERROR)
	{
		printf("failed to send\n");
	}
}

void Sock_Client::read(char* buf, int length)
{
	iResult = recv(ConSock, buf, length, MSG_WAITALL);
	if (iResult < 0)
	{
		printf("failed to recv\n");
	}
}
