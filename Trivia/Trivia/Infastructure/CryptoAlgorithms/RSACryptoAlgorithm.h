#pragma once
#include "./ICryptoAlgorithm.h"
#include "cryptopp/rsa.h"
#include "cryptopp/osrng.h"

class RSACryptoAlgorithm : public ICryptoAlgorithm
{
public:
	RSACryptoAlgorithm();

	std::string encrypt(std::string message) override;
	std::string encrypt(std::string message, std::string key) override;

	std::string decrypt(std::string message) override;
	
	std::string getPublicKey() const override;

private:
	void generateKeys();

	CryptoPP::RSA::PublicKey    publicKey;
	CryptoPP::RSA::PrivateKey   privateKey;
};

