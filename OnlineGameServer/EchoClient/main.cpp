#include <iostream>
#include <WinSock2.h>
#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT		8888
#define BUF_SIZE	1024

void main()
{
	WSAData wsaData{ 0 };
	::WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET s = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, 0);

	SOCKADDR_IN addr;
	::ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = ::inet_addr("127.0.0.1");
	addr.sin_port = ::htons(PORT);

	::connect(s, (SOCKADDR*)&addr, sizeof(addr));

	char buf[BUF_SIZE];
	::ZeroMemory(buf, sizeof(buf));

	std::cout << "input chatting : ";
	std::cin >> buf;

	::send(s, buf, std::strlen(buf), 0);
	::recv(s, buf, BUF_SIZE, 0);
	
	std::cout << "response: " << buf << std::endl;
	::closesocket(s);

	::WSACleanup();
}