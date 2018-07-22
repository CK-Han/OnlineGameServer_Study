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
	addr.sin_addr.S_un.S_addr = ADDR_ANY;
	addr.sin_port = PORT;

	::bind(s, (SOCKADDR*)&addr, sizeof(addr));
	::listen(s, 10);

	char buf[BUF_SIZE];

	//infinite roop
	while (true) {
		SOCKADDR_IN clientAddr;
		int clientAddrSize = sizeof(clientAddr);
		::ZeroMemory(buf, sizeof(buf));

		SOCKET client = ::accept(s, (SOCKADDR*)&clientAddr, &clientAddrSize);
		::recv(client, buf, BUF_SIZE, 0);
		
		std::cout << ::inet_ntoa(clientAddr.sin_addr) << ": " <<
			buf << std::endl;

		::closesocket(client);
	}

	::WSACleanup();
}