#pragma once
#include "./ICryptoAlgorithm.h"
#include <cryptopp/aes.h>

class AESCryptoAlgorithm : public ICryptoAlgorithm
{
public:
	AESCryptoAlgorithm();

	std::string encrypt(std::string message) override;
	std::string encrypt(std::string message, std::string key) override;

	std::string decrypt(std::string message) override;

	std::string getPublicKey() const override;

private:
	CryptoPP::SecByteBlock m_key;
};

