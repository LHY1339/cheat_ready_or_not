#pragma once
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <string>

class network
{
public:
	static network* get();
	void init();
	void main();
private:
	void timer_send();
	void recv_message();
	void sendmsg(const std::string& str);
	void handle(const std::string& str);
private:
	WSADATA wsadata;
	SOCKET client_socket;
	sockaddr_in server_addr;
	float wait_time = 0.0f;
};

