#pragma once
#include <string>

class ICryptoAlgorithm
{
public:
	virtual ~ICryptoAlgorithm() = default;

	virtual std::string encrypt(std::string message) = 0;
	virtual std::string encrypt(std::string message, std::string key) = 0;

	virtual std::string decrypt(std::string message) = 0;

	virtual std::string getPublicKey() const = 0;
};

