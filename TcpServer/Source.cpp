#include <Net\net.h>
#include <Net/socket.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace Net;
using namespace std::literals::chrono_literals;


void ClientHandler(const int SOCKET_PORT)
{
	if (Network::Initialize())
	{
		cout << "Thread id= " << this_thread::get_id() << "-> Successfully initialized!" << endl;

		Socket socket;
		if (socket.Create() == Result::Success)
		{
			cout << "Thread id= " << this_thread::get_id() << "-> Socket created successfully!" << endl;
			
			if (socket.Listen(IPEndpoint("0.0.0.0", SOCKET_PORT)) == Result::Success)
			{
				cout << "Thread id= " << this_thread::get_id() << "-> Socket successfully listening to port " << SOCKET_PORT << endl;
				while (true)
				{
					static int ClientID = 0;
					Socket newConnection;
					if (socket.Accept(newConnection) == Result::Success)
					{
						ClientID++;

						cout << "Thread id= " << this_thread::get_id() << "-> Connection accepted." << endl;

						string buffer = "";

						uint32_t num;
						Packet packet;

						while (true)
						{
							/*Result result = newConnection.Recv(packet);
							packet >> num;*/
							uint32_t bufferSize = 0;
							int result = newConnection.RecvPacket(&bufferSize, sizeof(uint32_t));

							if (result == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET || result != Result::Success)
							{
								break;
							}

							bufferSize = ntohl(bufferSize);
							buffer.resize(bufferSize);

							result = newConnection.RecvPacket(&buffer[0], bufferSize);

							if (result == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET || result != Result::Success)
							{
								break;
							}
							cout << "Thread id= " << this_thread::get_id() << "-> Client " << ClientID << ": Message: " << buffer << " [Bytes recived: " << buffer.size() << "]" << endl;

							buffer.clear();					// CLEAR BUFFER !!!!!!!!!

						}
						newConnection.Close();

						cout << "Client "<< ClientID << " disconected from the server!" << endl;
						--ClientID;
					}
					else
					{
						cerr << "Thread id= " << this_thread::get_id() << "-> Failed to accept." << endl;
					}
				}
			}
			else 
			{
				cerr << "Thread id= " << this_thread::get_id() << "-> Failet to bind socket" << endl;
			}
		}
		else
		{
			cerr << "Thread id= " << this_thread::get_id() << " Error:" << WSAGetLastError() << "Failed to create socket!" << endl;
		}
	}
	Network::Shutdown();
}

int main()
{
	cout << "Main ID:" << this_thread::get_id() << endl;

	thread firstSocket(ClientHandler, 6644);

	this_thread::sleep_for(2s);

	cout << endl;

	thread secondSocket(ClientHandler, 6645);

	this_thread::sleep_for(2s);

	firstSocket.join();
	secondSocket.join();

	system("pause");
	return 0;
}