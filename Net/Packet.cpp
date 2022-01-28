#include "Packet.h"


namespace Net
{
	void Net::Packet::Clear()
	{
		buffer.clear();
		extraction = 0;
	}

	void Net::Packet::Append(const void* data, uint32_t size)
	{
		buffer.insert(buffer.end(), (char*)data, (char*)data + size);
	}

	Packet& Net::Packet::operator <<(uint32_t data)
	{
		data = htonl(data);
		Append(&data, sizeof(uint32_t));

		return *this;
	}

	Packet& Net::Packet::operator >>(uint32_t& data)
	{
		data = *reinterpret_cast<uint32_t*>(&buffer[extraction]);
		data = ntohl(data);
		extraction += sizeof(uint32_t);

		return *this;
	}
	Packet& Packet::operator<<(const string& data)
	{
		*this << (uint32_t)data.size();
		Append(data.data(), data.size());

		return *this;
	}
	Packet& Packet::operator>>(string& data)
	{
		data.clear();

		uint32_t strSize = 0;
		*this >> strSize;

		data.resize(strSize);
		data.assign(&buffer[extraction], strSize);
		extraction += strSize;

		return *this;
	}
}
