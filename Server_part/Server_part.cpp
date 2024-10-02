
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable: 4996)

using namespace std;

SOCKET Connections[100];
int Counter = 0;

void ClientHandler(int index) {
	char msg[256];
	while (true)
	{
		recv(Connections[index], msg, sizeof(msg), NULL);
		for (int i = 0; i < Counter; i++) // отправка сообщения всем пользователям
		{
			if (index == i) continue;
			send(Connections[i], msg, sizeof(msg), NULL);
		}
	}
}


int main()
{

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1); // версия WinSock2.h
	if (WSAStartup(DLLVersion, &wsaData) != 0) // загрузка библиотеки и проверка загрузилась ли
	{
		cout << "Error" << endl;
		exit(1);
	}

	// заполнение информации об адресе сокета
	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(8080);
	addr.sin_family = AF_INET; // для интернет протоколов

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); // СОЗДАНИЕ СОКЕТА
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); // привязка адреса к сокету
	listen(sListen, SOMAXCONN); // прослушивание порта в ожидании клиента, SOMAXCONN- макс доп число запросов(можем поставить число например 3)

	// новый сокет для удержания подключения с клиентом
	SOCKET newConnection;
	for (int i = 0; i < 100; i++)
	{
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);
		if (newConnection == 0) //проверка подключился ли клиент к серверу
		{
			cout << "Error 2" << endl;
		}
		else
		{
			cout << "Client connected" << endl;
			char msg[256] = "hello";
			send(newConnection, msg, sizeof(msg), NULL); // отправка сообщения
		}
		Connections[i] = newConnection;
		Counter++;
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL); // создание нового потока

	}


	system("pause");
	return 0;
}

