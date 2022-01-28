#pragma once
#include "SocketHandle.h"
#include "Result.h"
#include "ipVersion.h"
#include <WinSock2.h>
#include "SocketOptions.h"
#include "IPEndpoint.h"
#include "Packet.h"

namespace Net
{
	class Socket
	{
	public:
		Socket(ipVersion IPv = ipVersion::IPv4, sock handle = INVALID_SOCKET);

		Result Create();

		Result Close();

		Result Bind(IPEndpoint endpoint);

		sock GetHandle();

		ipVersion GetVersion();

		Result Listen(IPEndpoint endpoint, int log = 10);

		Result Accept(Socket &socket);
		
		Result Connect(IPEndpoint endpoint);

		Result Send(const void* data, int& bytesSent, int numberOfBytes);

		Result Recv(void* destination, int& bytesRecv, int numberOfBytes);

		Result SendPacket(const void* data, int numberOfBytes);

		Result RecvPacket(void* destination, int numberOfBytes);

		Result Send(Packet &packet);

		Result Recv(Packet& packet);

	private:
		Result SetSocketOption(SocketOption option, BOOL value);
		ipVersion IPv = ipVersion::IPv4;
		sock handle = INVALID_SOCKET;
	};
}