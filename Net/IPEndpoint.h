#pragma once
#include "ipVersion.h"
#include <string>
#include <vector>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

namespace Net
{
	class IPEndpoint
	{
	public:
		IPEndpoint(const char* ip, unsigned int port);

		IPEndpoint(sockaddr* addr);

		ipVersion GetIPVersion();

		vector<uint8_t> GetIPByte();

		string GetHostname();

		string GetIPStr();

		unsigned int GetPort();

		sockaddr_in GetSockAddr();

		void PrintInfomation();
	private:
		ipVersion ipVer = ipVersion::Unknown;
		string ip_Str = "";
		string hostname = "";
		char localhost[256] = { 0 };
		vector<uint8_t> ip_byte;
		unsigned int port = 0;
	};
}
