#include <Net\net.h>
#include <Net/socket.h>
#include <iostream>
#include <WinSock2.h>
#include <thread>

using namespace std;
using namespace Net;
using namespace std::literals::chrono_literals;

void ClientHandler(const int SERVER_PORT)
{
	if (Network::Initialize())
	{
		cout << "Successfully initialized!" << endl;
		Socket Connection;
		if (Connection.Create() == Result::Success)
		{
			cout << "Socket created successfully!" << endl;

			if (Connection.Connect(IPEndpoint("127.0.0.1", SERVER_PORT)) == Result::Success)
			{
				cout << "Connected to server!" << endl;
				int result = Result::Success;

				if (result == Result::Success)
				{
					char buf[256] = { 0 };
					cout << "To server: ";
					string buffer = "hello";

				/*	uint32_t a, b, c = 0;
					a = 4;
					b = 3;
					c = 2;
					Packet packet;
					packet << a << b << c;*/
					while (true)
					{
						//Result result = Connection.Send(packet);
						getline(cin, buffer);
						uint32_t bufferSize = buffer.size();
						bufferSize = htonl(bufferSize);
						int result = Connection.SendPacket(&bufferSize, sizeof(uint32_t));

						if (result == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET)
						{
							break;
						}

						result = Connection.SendPacket(buffer.data(), buffer.size());
						cout << "To server: ";
					}
					Sleep(500);
				}
				Connection.Close();
			}
			else
			{
				cout << "Failed to connect to server." << endl;
			}
		}
		else
		{
			cerr << "Error:" << WSAGetLastError() << " Failed to create socket!" << endl;
		}
	}
	Network::Shutdown();
}

int main()
{
	int port = 0;
	cout << "Enter server port to connect(Server ports: 6644, 6645): ";
	cin >> port;

	if (port > 65535)
	{
		cout << "Port cannot be larger than 65535." << endl;
	}
	else
	{
		ClientHandler(port);
	}
	system("pause");
	return 0;
}