#pragma once
#include "../Codes.h"
#include <ctime>
#include <vector>
#include <memory>

class IRequestsHandler;

struct RequestInfo {
	CodeId id;
	time_t recivalTime;
	std::vector<unsigned char> buffer;
};

struct RequestResult {
	std::vector<unsigned char> buffer;
	std::unique_ptr<IRequestsHandler> newHandler;
};

class IRequestsHandler
{
public:
	virtual ~IRequestsHandler() = default;
	IRequestsHandler() = default;

	virtual bool isRequestsRelevant(const RequestInfo& requestInfo) const = 0;
	virtual RequestResult handleRequest(const RequestInfo& requestInfo) = 0;
	virtual void handleCrush() = 0;

};

