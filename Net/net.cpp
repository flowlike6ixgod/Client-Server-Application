#include "net.h"
#include <iostream>

using namespace std;

bool Net::Network::Initialize()
{
	WSADATA wsadata;
	int result = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (result != 0) 
	{
		cerr << "Failed to start up the winsock API." << endl;
		return false;
	}

	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2)
	{
		cerr << "Could not find a usable version of the winsock api dll." << endl;
		return false;
	}

	return true;
}

void Net::Network::Shutdown()
{
	WSACleanup();
}