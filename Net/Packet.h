#pragma once
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <iostream>
#include <vector>

using namespace std;

namespace Net
{
	class Packet
	{
	private:


	public:
		void Clear();

		void Append(const void* data, uint32_t size);

		Packet& operator << (uint32_t data);
		Packet& operator >> (uint32_t& data);
		Packet& operator << (const string& data);
		Packet& operator >>(string& data);

		uint32_t extraction = 0;
		vector<char> buffer;


	};
}