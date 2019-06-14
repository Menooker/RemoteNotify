// RemoteNotify.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")


__declspec(dllimport) HHOOK Inject(HWND hWnd);
__declspec(dllimport) bool HasMessage();
__declspec(dllimport) HHOOK UnInject();


void NotifyServer(const char* server_url)
{
	struct addrinfo hints, *result;
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_CANONNAME;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	auto rc = getaddrinfo(server_url, "8888", &hints, &result);
	if (rc != 0)
	{
		printf("getaddrinfo error\n");
		return;
	}
	SOCKET sclient = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (sclient == INVALID_SOCKET)
	{
		printf("invalid socket!");
		return;
	}
	if (connect(sclient, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR)
	{
		printf("connect error !");
		closesocket(sclient);
		return ;
	}

	char sendData = 'A';
	send(sclient, &sendData, 1, 0);
	closesocket(sclient);

}

int main(int argc, char* argv[])
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}
	if (argc != 2)
	{
		printf("Bad arguments\n");
		return 1;
	}
	HWND hExplorer = FindWindowExA(FindWindowA("Shell_TrayWnd", NULL), 0, "ReBarWindow32", NULL);
	hExplorer = FindWindowExA(hExplorer, 0, "MSTaskSwWClass", NULL);
	hExplorer = FindWindowExA(hExplorer, 0, "MSTaskListWClass", NULL);
	std::cout << Inject(hExplorer) << " Inject!\n";
    std::cout << hExplorer <<" Hello World!\n";
	int msg_cnt = 0;
	for (;;)
	{
		Sleep(1000 * 3);
		if (HasMessage())
		{
			msg_cnt++;
			printf("Got message\n");
			if (msg_cnt>=2)
				NotifyServer(argv[1]);
		}
		else
		{
			msg_cnt = 0;
		}
	}
}
