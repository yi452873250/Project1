#ifndef ESAY_TCP_CLIENT_HPP
#define ESAY_TCP_CLIENT_HPP
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#define SOCKET int 
#define INVALID_SOCKET    (SOCKET)(~0)
#define SOCKET_ERROR		(-1)
#endif // _WIN32

#include <stdio.h>
class EasyTcpClient
{
	
public:
	EasyTcpClient() {
		_sock = INVALID_SOCKET;
	}
	virtual ~EasyTcpClient() {
		Close();
	}
	void InitSocket() {
#ifdef _WIN32
		WORD ver = MAKEWORD(2, 2);
		WSADATA data;
		WSAStartup(ver, &data);
#endif // _WIN32
		if (_sock != INVALID_SOCKET) {
			printf("重复连接关闭之前的连接\n");
			Close();
		}
		_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == _sock) {
			printf("建立socket失败\n");
		}
		else {
			printf("建立socket成功\n");
		}
	}
	int Connect(char* ip, unsigned short port) {
		if (ip == NULL||port<1000) {
			return -1;
		}
		if (_sock == INVALID_SOCKET) {
			InitSocket();
		}
		sockaddr_in _sin = {};
		_sin.sin_family = AF_INET;
		_sin.sin_port = htons(port);
#ifdef _WIN32
		_sin.sin_addr.S_un.S_addr = inet_addr(ip);
#else
		_sin.sin_addr.s_addr = inet_addr(ip);
#endif // _WIN32
		int ret = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
		if (SOCKET_ERROR == ret) {
			printf("链接服务器失败\n");
		}
		else {
			printf("连接服务器成功\n");
		}
		return ret;

	}

	void Close() {
		if (_sock != INVALID_SOCKET) {
#ifdef _WIN32
			closesocket(_sock);
			WSACleanup();
#else
			close(_sock);
#endif // _WIN32
		}
	}

private:
	SOCKET _sock;
};


#endif // !1
