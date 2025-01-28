#include "RSACryptoAlgorithm.h"
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>
#include <algorithm>

RSACryptoAlgorithm::RSACryptoAlgorithm()
{
	this->generateKeys();
}

std::string RSACryptoAlgorithm::encrypt(std::string message)
{
	return this->encrypt(message, this->getPublicKey());
}

std::string RSACryptoAlgorithm::decrypt(std::string message)
{
	std::string recovered;

	using namespace CryptoPP;

	std::string decoded;

	StringSource ss1(message, true,
		new Base64Decoder(
			new StringSink(decoded)
		)
	);

	CryptoPP::RSAES_OAEP_SHA_Decryptor d(privateKey);
	CryptoPP::AutoSeededRandomPool rng;

	CryptoPP::StringSource ss(decoded, true,
		new CryptoPP::PK_DecryptorFilter(rng, d,
			new CryptoPP::StringSink(recovered)
		)
	);

	return recovered;
}

std::string RSACryptoAlgorithm::encrypt(std::string message, std::string key)
{
	using namespace CryptoPP;

	std::string cipher;
	AutoSeededRandomPool prng;
	try {
		ByteQueue queue;
		Base64Decoder decoder(new Redirector(queue));
		decoder.Put((const byte*)key.data(), key.size());
		decoder.MessageEnd();

		RSA::PublicKey rsaPublick;
		rsaPublick.BERDecodePublicKey(queue, false, (size_t)queue.MaxRetrievable());

		CRYPTOPP_ASSERT(queue.IsEmpty());

		bool valid = rsaPublick.Validate(prng, 3);
		if (!valid)
			cipher = "RSA public key is not valid";

		RSAES_OAEP_SHA_Encryptor e(rsaPublick);
		StringSource ss(message, true,
			new PK_EncryptorFilter(prng, e,
				new StringSink(cipher)
			)
		);
	}
	catch (CryptoPP::Exception& e) {
		cipher = e.what();
	}

	std::string base64encodedCipher;

	StringSource ss1(cipher, true,
		new Base64Encoder(
			new StringSink(base64encodedCipher)
		)
	);

	base64encodedCipher.erase(std::remove(base64encodedCipher.begin(), base64encodedCipher.end(), '\n'), base64encodedCipher.end());
	base64encodedCipher.erase(std::remove(base64encodedCipher.begin(), base64encodedCipher.end(), '\r'), base64encodedCipher.end());

	return base64encodedCipher;
}

std::string RSACryptoAlgorithm::getPublicKey() const
{
	using namespace CryptoPP;

	std::string result;
	Base64Encoder encoder(new StringSink(result));
	this->publicKey.DEREncodePublicKey(encoder);
	encoder.MessageEnd();

	result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
	result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());
	return result;
}

void RSACryptoAlgorithm::generateKeys()
{
	using namespace CryptoPP;

	CryptoPP::AutoSeededRandomPool rng;

	InvertibleRSAFunction params;
	params.GenerateRandomWithKeySize(rng, 2048);

	// Create Keys
	RSA::PrivateKey privateKeyGenerated(params);
	RSA::PublicKey publicKeyGenerated(params);

	this->privateKey = privateKeyGenerated;
	this->publicKey = publicKeyGenerated;
}
