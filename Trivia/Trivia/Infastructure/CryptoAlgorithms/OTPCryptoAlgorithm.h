#pragma once
#include "./ICryptoAlgorithm.h"

class OTPCryptoAlgorithm : public ICryptoAlgorithm
{
public:
	std::string encrypt(std::string message) override;
	std::string decrypt(std::string message) override;

	std::string encrypt(std::string message, std::string key) override;

	std::string getPublicKey() const override;

private:
	std::string generateKey(std::string message) const;
};

