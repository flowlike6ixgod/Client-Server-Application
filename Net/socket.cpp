#include "socket.h"
#include <assert.h>
#include <iostream>

using namespace std;

namespace Net
{
	/*Socket cunstructor*/
	Socket::Socket(ipVersion IPv, sock handle)
		: IPv(IPv), handle(handle)
	{
		assert(IPv == ipVersion::IPv4);
	}
	/*Create socket*/
	Result Socket::Create()
	{
		assert(IPv == ipVersion::IPv4);
		if (handle != INVALID_SOCKET)
		{
			return Result::Generic_Error;
		}

		handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Create socket

		if (handle == INVALID_SOCKET)
		{
			int error = WSAGetLastError();
			return Result::Generic_Error;
		}

		if (SetSocketOption(SocketOption::TCP_NoDelay, TRUE) != Result::Success)
		{
			return Result::Generic_Error;
		}

		return Result::Success;
	}
	/*Close socket*/
	Result Socket::Close()
	{
		if (handle != INVALID_SOCKET)
		{
			return Result::Generic_Error;
		}
		int result = closesocket(handle);
		if (result != 0)
		{
			int result = WSAGetLastError();
			cerr << "Error:" << result << endl;
			return Result::Generic_Error;
		}
		handle = INVALID_SOCKET;
		return Result::Success;
	}
	/*Binding socket*/
	Result Socket::Bind(IPEndpoint endpoint)
	{
		sockaddr_in addr = endpoint.GetSockAddr();
		int result = bind(handle, (sockaddr*)&addr, sizeof(sockaddr_in));

		if (result != 0)
		{
			int error = WSAGetLastError();
			cerr << "Error: " << error << endl;
			return Result::Generic_Error;
		}
		return Result::Success;
	}
	sock Socket::GetHandle()
	{
		return handle;
	}
	ipVersion Socket::GetVersion()
	{
		return IPv;
	}
	/*Listening socket*/
	Result Socket::Listen(IPEndpoint endpoint, int log)
	{
		if (Bind(endpoint) != Result::Success)
		{
			return Result::Generic_Error;
		}

		int result = listen(handle, log);

		if (result != 0)
		{
			int error = WSAGetLastError();
			cerr << "Error: " << error << endl;
			return Result::Generic_Error;
		}

		return Result::Success;
	}
	Result Socket::Accept(Socket& socket)
	{
		sockaddr_in addr = {};
		int lenght = sizeof(sockaddr_in);
		sock acceptConnection = accept(handle, (sockaddr*)&addr, &lenght);
		if (acceptConnection == INVALID_SOCKET)
		{
			int error = WSAGetLastError();
			cerr << "Error: " << error << endl;
			return Result::Generic_Error;
		}
		IPEndpoint connectionEndpoint((sockaddr*)&addr);
		cout << "Connection accepted!" << endl;
		connectionEndpoint.PrintInfomation();
		socket = Socket(ipVersion::IPv4, acceptConnection);


		return Result::Success;
	}
	/*Socket connection*/
	Result Socket::Connect(IPEndpoint endpoint)
	{
		sockaddr_in addr = endpoint.GetSockAddr();
		int result = connect(handle, (sockaddr*)&addr, sizeof(sockaddr_in));

		if (result != 0)
		{
			int error = WSAGetLastError();
			cerr << "Error: " << error << endl;
			return Result::Generic_Error;
		}

		return Result::Success;
	}
	/*Sending data*/
	Result Socket::Send(const void* data, int& bytesSent, int numberOfBytes)
	{
		bytesSent = send(handle, (const char*)data, numberOfBytes, 0);

		if (bytesSent == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			cerr << "Error: " << error << endl;
			return Result::Generic_Error;
		}

		return Result::Success;
	}
	/*Reciving data*/
	Result Socket::Recv(void* destination, int& bytesRecv, int numberOfBytes)
	{
		bytesRecv = recv(handle, (char*)destination, numberOfBytes, 0);

		if (bytesRecv == 0)
		{
			return Result::NotImplemented;
		}
		if (bytesRecv == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			/*cerr << "Error: " << error << endl;*/
			return Result::Generic_Error;
		}

		return Result::Success;
	}

	// Send Partial Packets
	Result Socket::SendPacket(const void* data, int numberOfBytes)
	{
		int totalBytes = 0;
		while (totalBytes < numberOfBytes)
		{
			int bytesRemaining = numberOfBytes - totalBytes;
			int bytesSent = 0;
			char* buffer = (char*)data + totalBytes;
			Result result = Send(buffer, bytesSent, bytesRemaining );

			if (result != Result::Success)
			{
				return Result::Generic_Error;
			}

			totalBytes += bytesSent;
		}

		return Result::Success;
	}

	// Recive Partial Packets
	Result Socket::RecvPacket(void* destination, int numberOfBytes)
	{
		int totalBytes = 0;
		while (totalBytes < numberOfBytes)
		{
			int bytesRemaining = numberOfBytes - totalBytes;
			int bytesRecived = 0;
			char* buffer = (char*)destination + totalBytes;
			Result result = Recv(buffer, bytesRecived, bytesRemaining);

			if (result != Result::Success)
			{
				return Result::Generic_Error;
			}

			totalBytes += bytesRecived;
		}

		return Result::Success;
	}
	Result Socket::Send(Packet& packet)
	{
		uint32_t PacketSize = htonl(packet.buffer.size());
		Result result = SendPacket(&PacketSize, sizeof(uint32_t));

		if (result != Result::Success)
		{
			return Result::Generic_Error;
		}

		result = SendPacket(packet.buffer.data(), packet.buffer.size());

		if (result != Result::Success)
		{
			return Result::Generic_Error;
		}

		return Result::Success;
	}
	Result Socket::Recv(Packet& packet)
	{
		packet.Clear();  

		uint32_t PacketSize = 0;
		Result result = RecvPacket(&PacketSize, sizeof(uint32_t));

		if (result != Result::Success)
		{
			return Result::Generic_Error;
		}

		uint32_t bufferSize = htonl(PacketSize);
		packet.buffer.resize(bufferSize);
		result = RecvPacket(&packet.buffer[0], bufferSize);

		if (result != Result::Success)
		{
			return Result::Generic_Error;
		}

		return Result::Success;
	}
	/*Set socket options*/
	Result Socket::SetSocketOption(SocketOption option, BOOL value)
	{
		int result = 0;
		switch (option)
		{
		case SocketOption::TCP_NoDelay:
			result = setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, (const char*)&value, sizeof(value));
			break;
		default:
			return Result::Generic_Error;
			break;
		}
		if (result != 0)
		{
			int error = WSAGetLastError();
			cout << "Error: " << error << endl;
			return Result::Generic_Error;
		}

		return Result::Success;
	}
}