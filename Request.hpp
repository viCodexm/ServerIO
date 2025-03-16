
#pragma once
#include <iostream>

using namespace std;

enum struct RequestType { READ, WRITE, UNDEF };

struct Request {
	int id;
	int timestamp;
	RequestType type;
	int address;
	int size;
	int endtime;

	Request() : id(0), timestamp(0), type(RequestType::UNDEF), address(0), size(0), endtime(0) {};

	Request(int id, int timestamp, RequestType type, int address, int size)
		: id(id), timestamp(timestamp), type(type), address(address), size(size), endtime(0) {
	};

	friend istream& operator>>(istream& stream, Request& req);
	friend ostream& operator<<(ostream& stream, Request& req);

	static RequestType strToReqType(const string& str);
	static string reqTypeToStr(RequestType reqt);

	int latency();

	bool operator==(const Request& other) const;
};
