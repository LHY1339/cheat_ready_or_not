#include "network.h"
#include "gdefine.h"
#include "gvalue.h"

#include <iostream>
#include "save.h"

network* network::get()
{
	static network inst;
	return &inst;
}

void network::init()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
	{
		printf("WSAStartup(MAKEWORD(2, 2), &wsadata) != 0\n");
		return;
	}

	client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (client_socket == INVALID_SOCKET)
	{
		printf("client_socket == INVALID_SOCKET\n");
		return;
	}
	u_long mode = 1;
	ioctlsocket(client_socket, FIONBIO, &mode);

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
}

void network::main()
{
	timer_send();
	recv_message();
}

void network::timer_send()
{
	wait_time += gvalue::delta_time;
	if (wait_time < 2.0f)
	{
		return;
	}
	wait_time = 0.0f;
	sendmsg(CHEAT_HEART);
}

void network::recv_message()
{
	char buffer[1024];
	sockaddr_in buf_addr;
	int buf_addr_size = sizeof(buf_addr);
	int recvLen = recvfrom(client_socket, buffer, sizeof(buffer) - 1, 0, reinterpret_cast<sockaddr*>(&buf_addr), &buf_addr_size);
	if (recvLen != SOCKET_ERROR)
	{
		buffer[recvLen] = '\0';
		handle(buffer);
	}
}

void network::sendmsg(const std::string& str)
{
	sendto(
		client_socket,
		str.c_str(),
		static_cast<int>(strlen(str.c_str())),
		0,
		reinterpret_cast<sockaddr*>(&server_addr),
		static_cast<int>(sizeof(server_addr))
	);
}

void network::handle(const std::string& str)
{
	gvalue::version = str;
}
