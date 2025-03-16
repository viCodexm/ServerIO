
#include "Request.hpp"


RequestType Request::strToReqType(const string& str) {
	if (str == "READ") return RequestType::READ;
	else if (str == "WRITE") return RequestType::WRITE;
	return RequestType::UNDEF;
}


string Request::reqTypeToStr(RequestType reqt) {
	switch (reqt) {
	case RequestType::READ: return "READ";
	case RequestType::WRITE: return "WRITE";
	default: return "UNDEF";
	}
}


istream& operator>>(istream& is, Request& req) {
	string typeStr;
	is >> req.id >> req.timestamp >> typeStr >> req.address >> req.size;
	req.type = Request::strToReqType(typeStr);

	return is;
}


ostream& operator<<(ostream& is, Request& req) {
	is << "Request ID: " << req.id << " "
		<< "Timestamp: " << req.timestamp << " usec "
		<< "Type: " << Request::reqTypeToStr(req.type) << ", "
		<< "Address: " << req.address << ", "
		<< "Size: " << req.size;

	return is;
}

int Request::latency() {
	return size * (type == RequestType::WRITE ? 1 : type == RequestType::READ ? 2 : 0);
}

bool Request::operator==(const Request& other) const {
	return id == other.id;
}