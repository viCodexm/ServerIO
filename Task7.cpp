
#include <algorithm>
#include <iostream>
#include <numeric>
#include <fstream>
#include <vector>
#include <assert.h>
#include <set>

#include "Request.hpp"
#include "AVL.hpp"

using namespace std;

#define all(v) v.begin(), v.end()


struct Processor {
	AVL intervals;
	vector<Request> working;
	vector<Request> waiting;

	int currentTimestamp = 0;
};

void AddRequestToWork(Request& request, Processor* p) {
	request.endtime = p->currentTimestamp + request.latency();
	p->working.push_back(request);
	if (request.type == RequestType::WRITE) {
		p->intervals.insertInterval(request.address, request.address + request.size + 1);
	}
}

void DirectNewRequest(Request& request, Processor* p, int MAX_WORKLOAD) {
	if (p->working.size() >= MAX_WORKLOAD) {
		p->waiting.push_back(request);
		return;
	}

	int addressStart = request.address;
	int addressEnd = addressStart + request.size;

	if (p->intervals.overlaps(addressStart, addressEnd)) {
		p->waiting.push_back(request);
		return;
	}

	AddRequestToWork(request, p);
}

Request FindBestSuitedWaitingRequest(Processor& p) {
	Request best; best.timestamp = INT_MAX;
	for (Request& request : p.waiting) {
		int addressStart = request.address;
		int addressEnd = addressStart + request.size;

		if (request.timestamp < best.timestamp && !p.intervals.overlaps(addressStart, addressEnd)) {
			best = request;
		}
	}
	return best;
}

void DeleteWorkingRequest(Request& request, Processor* p, vector<int> *latencies) {
	latencies->push_back(p->currentTimestamp - request.timestamp);
	if (request.type == RequestType::WRITE) {
		p->intervals.deleteInterval(request.address, request.address + request.size + 1);
	}
	auto it = remove(all(p->working), request);
	p->working.erase(it, p->working.end());
}

void DeleteWaitingRequest(Request& request, Processor* p) {
	auto it = remove(all(p->waiting), request);
	p->waiting.erase(it, p->waiting.end());
}

void DeleteAllCompletedRequests(Processor* processor, vector<int>* latencies) {
	vector<Request> trash;
	for (Request& e : processor->working) {
		if (e.endtime <= processor->currentTimestamp) {
			trash.push_back(e);
		}
	}
	if (!trash.empty()) {
		for (Request& e : trash) {
			DeleteWorkingRequest(e, processor, latencies);
		}
	}
}

void PrintLatencyStats(vector<int> latencies, int MAX_WORKLOAD) {
	sort(all(latencies));
	int size = latencies.size();
	float average = accumulate(all(latencies), 0.0) / size;
	float median = (size & 1) ? latencies[size / 2] : ((float)latencies[size / 2 - 1] + latencies[size / 2]) / 2.0;

	cout << "N: " << MAX_WORKLOAD << "\n"
		<< "Median: " << median << "\n"
		<< "Average: " << average << "\n"
		<< "Min: " << latencies.front() << "\n"
		<< "Max: " << latencies.back() << "\n\n";
}

void PrintProcessorState(Processor& processor) {
	cout << "Timestamp: " << processor.currentTimestamp << " inwork: ";
	for (Request& r : processor.working) {
		cout << r.id << " ";
	}
	cout << " ||||== waiting: ";
	for (Request& r : processor.waiting) {
		cout << r.id << " ";
	}
	cout << "\n";
}

void solve(vector<Request> requests, int MAX_WORKLOAD) {
	vector<int> latencies;


	Processor processor;

	int reqId = 0;
	for (processor.currentTimestamp = 0; reqId < requests.size() || !processor.working.empty() || !processor.waiting.empty(); ++processor.currentTimestamp) {

		// catch all new requests
		while (reqId < requests.size() && processor.currentTimestamp == requests[reqId].timestamp) {
			DirectNewRequest(requests[reqId], &processor, MAX_WORKLOAD);
			++reqId;
		}

		// Verbose output
		//PrintProcessorState(processor);

		DeleteAllCompletedRequests(&processor, &latencies);

		while (processor.working.size() < MAX_WORKLOAD) {
			// No requests for handling..
			if (processor.waiting.empty()) {
				break;
			}
			else {
				Request request = FindBestSuitedWaitingRequest(processor);
				// No requests in Waiting can be handled..
				if (request.type == RequestType::UNDEF) {
					break;
				}
				else {
					assert(processor.currentTimestamp + request.latency() > processor.currentTimestamp);
					AddRequestToWork(request, &processor);
					DeleteWaitingRequest(request, &processor);
				}
			}
		}
	}

	assert(processor.working.empty());
	assert(processor.waiting.empty());

	PrintLatencyStats(latencies, MAX_WORKLOAD);
}

int main() {

	ifstream input("requests.txt");
	if (!input.is_open()) {
		cout << "File did not open.\n";
		return -1;
	}

	vector<Request> requests;
	Request req;
	while (input >> req) {
		requests.push_back(req);
	}

	//cout << "\n\n--MEASURMENTS--\n";
	solve(requests, 1);
	solve(requests, 5);
	solve(requests, 10);
	


	return 0;
}