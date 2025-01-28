#include "OTPCryptoAlgorithm.h"
#include <random>

std::string OTPCryptoAlgorithm::encrypt(std::string message)
{
	std::string key = generateKey(message);
	std::string encryptedMessage;

	for (int i = 0; i < message.size(); i++) {
		if (message[i] >= 'A' && message[i] <= 'Z') {
			encryptedMessage.push_back((char)('A' + (message[i] - 'A' + key[i] - 'a') % 26));
		}
		else if (message[i] >= 'a' && message[i] <= 'z') {
			encryptedMessage.push_back((char)('a' + (message[i] - 'a' + key[i] - 'a') % 26));
		}	
		else {
			encryptedMessage.push_back(message[i]);
		}	
	}

	return encryptedMessage + key;
}

std::string OTPCryptoAlgorithm::decrypt(std::string message)
{
	std::string key = message.substr(message.size() / 2);
	std::string cipher  = message.substr(0, message.size() / 2);
	std::string decryptedMessage;

	for (int i = 0; i < cipher.size(); i++) {
		if (cipher[i] >= 'A' && cipher[i] <= 'Z') {
			if ((message[i] - 'A' - key[i] + 'a') < 0) {
				decryptedMessage.push_back('A' + (char)((message[i] - 'A' - key[i] + 'a') + 26));
			}
			else {
				decryptedMessage.push_back('A' + (char)(message[i] - 'A' - key[i] + 'a'));
			}
		}
		else if (message[i] >= 'a' && message[i] <= 'z') {
			if ((message[i] - 'a' - key[i] + 'a') < 0) {
				decryptedMessage.push_back('a' + (char)((message[i] - 'a' - key[i] + 'a') + 26));
			}
			else {
				decryptedMessage.push_back('a' + (char)(message[i] - 'a' - key[i] + 'a'));
			}
		}
		else {
			decryptedMessage.push_back(message[i]);
		}
	}

	return decryptedMessage;
}

std::string OTPCryptoAlgorithm::encrypt(std::string message, std::string key)
{
	return std::string("not implemented!");
}

std::string OTPCryptoAlgorithm::getPublicKey() const
{
	return this->generateKey("123");
}

std::string OTPCryptoAlgorithm::generateKey(std::string message) const
{
	std::string key;

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0, 26);

	for (int i = 0; i < message.size(); i++) {
		key += (char)('a' + (int)dist(mt));
	}

	return key;
}
