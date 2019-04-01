#include<iostream>
#include<winsock2.h>
#include<thread>
#include<string>
using namespace std;
#pragma comment(lib,"ws2_32.lib")

const u_short	SERVER_LISTEN_PORT = 8888;
const char *	SERVER_LISTEN_ADDRESS = "10.20.4.218";
const int		MaximumIncomingConnections = 10;				//socket�����Ŷӵ�������Ӹ���
const int		maxBufSize = 1024;								//�����뷢�͵�����ֽ���

void  serverThread(SOCKET* socket, const sockaddr_in remoteAddr)
{
	SOCKET* clientSocket = socket;			//�����̴߳������Ŀͻ���socket����
	int recvSize = 0;
	char recvData[maxBufSize] = "";
	char sendData[maxBufSize] = "";
	while (true)
	{
		int recvSize = recv(*clientSocket, recvData, sizeof(recvData), 0);
		if (recvSize > 0)
		{
			cout << "receive data is : " << recvData << "   that from socket :" << clientSocket << endl;
		} 
		else
		{
			cout << "receive is over" << endl;
			break;
		}
		memset(recvData, 0, sizeof(recvData));

		/*
		//send
		cout << "please enter the information to be send to the client: " << endl;
		gets_s(sendData);
		int sendSize = send(*clientSocket, sendData, sizeof(sendData), 0);
		if (sendSize < 0)
		{
		cout << "fail to send " << endl;
		}
		memset(sendData, 0, sizeof(sendData));
		*/
	}
	cout << inet_ntoa(remoteAddr.sin_addr) << "is disconnected" << endl;
	closesocket(*clientSocket);
	delete(clientSocket);
}


void main()
{
	//�����׽��ֿ�
	WORD socketVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(socketVersion, &wsaData) != 0)
	{
		cout << "WSAStartup is failed" << endl;
		return;
	}

	//�����׽���
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		cout << "establish socket is failed" << endl;
		return;
	}

	//��socket  --> ip+port
	sockaddr_in socketIn;					//Э���ַ
	socketIn.sin_family = AF_INET;
	socketIn.sin_port = htons(SERVER_LISTEN_PORT);
	socketIn.sin_addr.S_un.S_addr = INADDR_ANY;
	//socketIn.sin_addr.S_un.S_addr = inet_addr(SERVER_LISTEN_ADDRESS);
	if (::bind(listenSocket, (LPSOCKADDR)&socketIn, sizeof(socketIn)) == SOCKET_ERROR)
	{
		cout << "socket bind error" << endl;
		return;
	}

	//����
	if (listen(listenSocket, MaximumIncomingConnections) == SOCKET_ERROR)
	{
		cout << "listen  error" << endl;
		return;
	}

	while (true)
	{
		//ѭ�����տͻ����������󲢴��������߳�
		SOCKET* clientSocket = new SOCKET;
		sockaddr_in remoteAddr;
		int nAddrLen = sizeof(remoteAddr);

		*clientSocket = accept(listenSocket, (sockaddr*)&remoteAddr, &nAddrLen);
		if (*clientSocket == SOCKET_ERROR)
		{
			cout << "accept error" << endl;
			return;
		}

		cout << inet_ntoa(remoteAddr.sin_addr) << "is connected" << endl;
		thread t(serverThread, clientSocket,remoteAddr);
		t.detach();
	}

	closesocket(listenSocket);
	WSACleanup();
}