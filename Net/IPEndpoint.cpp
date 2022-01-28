#include <iostream>
#include "IPEndpoint.h"
#include <assert.h>

namespace Net
{
	IPEndpoint::IPEndpoint(const char* ip, unsigned int port)
	{
		this->port = port;

		in_addr addr;
		int result = inet_pton(AF_INET, ip, &addr);

		if (result == 1)
		{
			if (addr.S_un.S_addr != INADDR_NONE)
			{
				ip_Str = ip;
				hostname = ip;

				ip_byte.resize(sizeof(ULONG));
				memcpy(&ip_byte[0], &addr.S_un.S_addr, sizeof(ULONG));
				ipVer = ipVersion::IPv4;
				return;
			}
		}
		/*We can use domain names instead of default IPv4 address (example: www.reddit.com)*/
		addrinfo prompt = {};
		prompt.ai_family = AF_INET;
		addrinfo* hostinfo;
		result = getaddrinfo(ip, NULL, &prompt, &hostinfo);
		if (result == 0)
		{
			sockaddr_in* host_addr = reinterpret_cast<sockaddr_in*>(hostinfo->ai_addr);
			ip_Str.resize(16);
			inet_ntop(AF_INET, &host_addr->sin_addr, &ip_Str[0], 16);

			hostname = ip;

			ULONG longIP = host_addr->sin_addr.S_un.S_addr;
			ip_byte.resize(sizeof(ULONG));
			memcpy(&ip_byte[0], &longIP, sizeof(ULONG));

			ipVer = ipVersion::IPv4;

			freeaddrinfo(hostinfo);
			return;
		}

	}
	/*Retrieving connection info from Accept*/
	IPEndpoint::IPEndpoint(sockaddr* addr)
	{
		assert(addr->sa_family == AF_INET);

		sockaddr_in* addrv4 = reinterpret_cast<sockaddr_in*>(addr);
		ipVer = ipVersion::IPv4;
		port = ntohs(addrv4->sin_port);
		ip_byte.resize(sizeof(ULONG));
		memcpy(&ip_byte[0], &addrv4->sin_addr, sizeof(ULONG)); 
		ip_Str.resize(16);
		inet_ntop(AF_INET, &addrv4->sin_addr, &ip_Str[0], 16);

		gethostname(localhost, sizeof(localhost));

		hostname = ip_Str;
	}

	ipVersion IPEndpoint::GetIPVersion()
	{
		return ipVer;
	}

	string IPEndpoint::GetIPStr()
	{
		return ip_Str;
	}

	string IPEndpoint::GetHostname()
	{
		return hostname;
	}

	vector<uint8_t> IPEndpoint::GetIPByte()
	{
		return ip_byte;
	}

	unsigned int IPEndpoint::GetPort()
	{
		return port;
	}

	sockaddr_in IPEndpoint::GetSockAddr()
	{
		assert(ipVer == ipVersion::IPv4);
		sockaddr_in addr = {};
		addr.sin_family = AF_INET;
		memcpy(&addr.sin_addr, &ip_byte[0], sizeof(ULONG));
		addr.sin_port = htons(port);

		return addr;
	}
	/*Print information about connections*/
	void IPEndpoint::PrintInfomation()
	{
	
		if (ipVer == ipVersion::IPv4)
		{
			cout << "IP version IPv4." << endl;
		}
		if (ipVer == ipVersion::IPv6)
		{
			cout << "IP version IPv6." << endl;
		}
		if (ipVer == ipVersion::Unknown)
		{
			cout << "IP version Unknown." << endl;
		}
		cout << "Hostname: " << hostname << endl;
		cout << "Local computer name: " << localhost << endl;
		cout << "IP: " << ip_Str << endl;
		cout << "Port: " << port << endl;
	}
}
