#include <iostream>
#include <set>
#include <WinSock2.h>
#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT			8888
#define BUFSIZE			255

void main()
{
	WSADATA wsaData;
	::WSAStartup(MAKEWORD(2, 2), &wsaData);

	//flag�� OVERLAPPED�� ���, �� �������� ��������� Ŭ���̾�Ʈ ���� ���� OVERLAPPED�̴�.
	SOCKET acceptSocket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, 0);
	SOCKADDR_IN addr;
	::ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = ::htons(PORT);
	addr.sin_addr.S_un.S_addr = ::htonl(INADDR_ANY);

	::bind(acceptSocket, (SOCKADDR*)&addr, sizeof(addr));
	::listen(acceptSocket, 10);

	FD_SET readSet;
	FD_SET writeSet;

	std::set<SOCKET> clients;
	char buf[BUFSIZE];
	while (true) {
		FD_ZERO(&readSet);
		FD_ZERO(&writeSet);
		FD_SET(acceptSocket, &readSet);
		::ZeroMemory(buf, sizeof(buf));

		for (auto& sock : clients) {
			FD_SET(sock, &readSet);
		}

		::select(0, &readSet, &writeSet, nullptr, nullptr);
		// 1. accept ���� ó��
		if (FD_ISSET(acceptSocket, &readSet)) {
			SOCKADDR_IN clientAddr;
			int clientAddrSize = sizeof(clientAddr);
			SOCKET client = ::accept(acceptSocket, (SOCKADDR*)&clientAddr, &clientAddrSize);
			
			std::cout << ::inet_ntoa(clientAddr.sin_addr) << " client ����\n";
			clients.insert(client);			
		}

		// 2. clients ����� ó��
		for (auto iter = clients.cbegin(); iter != clients.cend();) {
			if (FD_ISSET(*iter, &readSet)) {
				int recvResult = ::recv(*iter, buf, BUFSIZE, 0);
				
				if (recvResult == 0) {
					iter = clients.erase(iter);
					std::cout << "���� ����\n";
					continue;
				}

				std::cout << "client says: " << buf << std::endl;
				::send(*iter, buf, recvResult + 1, 0);	
			}

			++iter;
		}
	}
	
	::WSACleanup();
}