#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable: 4996)

using namespace std;

SOCKET Connection;

void ClientHandler() 
{
	char msg[256];
	while (true)
	{
		recv(Connection, msg, sizeof(msg), NULL); // получение сообщения от сервера
		cout << msg << endl;
	}
}

int main()
{

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1); 
	if (WSAStartup(DLLVersion, &wsaData) != 0) 
	{
		cout << "Error" << endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(8080);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL); // сокет для соединения с сервером
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) // подсоединение к серверу
	{
		cout << "Error: No connect whith server" << endl;
		return 1;
	}
	cout << "Connected" << endl;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);


	char msg2[256];
	while (true)
	{
		cin.getline(msg2, sizeof(msg2));
		send(Connection, msg2, sizeof(msg2), NULL);
		Sleep(10);
	}

	system("pause");

	return 0;

}
