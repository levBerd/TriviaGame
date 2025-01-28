#include "AESCryptoAlgorithm.h"
#include <cryptopp/modes.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <cryptopp/base64.h>


AESCryptoAlgorithm::AESCryptoAlgorithm() : m_key(CryptoPP::AES::DEFAULT_KEYLENGTH)
{
	CryptoPP::AutoSeededRandomPool prng;
	prng.GenerateBlock(m_key, m_key.size());
	
}

std::string AESCryptoAlgorithm::encrypt(std::string message)
{
	std::string cipherText;

    // generate iv
    CryptoPP::AutoSeededRandomPool prng;
    CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
    prng.GenerateBlock(iv, iv.size());

    // ecnrypt
	CryptoPP::AES::Encryption aesEncryption(m_key, CryptoPP::AES::DEFAULT_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);
	CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(cipherText));
	stfEncryptor.Put(reinterpret_cast<const unsigned char*>(message.c_str()), message.length());
	stfEncryptor.MessageEnd();

    //HexEncode IV
	std::string ivString;
    CryptoPP::HexEncoder encoder(new CryptoPP::FileSink(std::cout));
    encoder.Detach(new CryptoPP::StringSink(ivString));
    encoder.Put(iv, iv.size());
    encoder.MessageEnd();

    //HexEncode ciphertxt
    std::string ciphertxt;
    encoder.Detach(new CryptoPP::StringSink(ciphertxt));
    encoder.Put((const CryptoPP::byte*)&cipherText[0], cipherText.size());
    encoder.MessageEnd();

	return ivString + "::" + ciphertxt;
}

std::string AESCryptoAlgorithm::encrypt(std::string message, std::string key)
{
    using namespace CryptoPP;

    AutoSeededRandomPool prng;
    SecByteBlock iv(AES::BLOCKSIZE);

    CryptoPP::SecByteBlock keyByteBlock(key.length()/2);
    CryptoPP::HexDecoder decoder;
    decoder.Put((const byte*)key.data(), key.size());
    decoder.MessageEnd();
    decoder.Get(keyByteBlock, keyByteBlock.size());

    // Generate IV
    prng.GenerateBlock(iv, iv.size());
    std::string cipher, recovered;


    CBC_Mode< AES >::Encryption e;
    e.SetKeyWithIV(keyByteBlock, keyByteBlock.size(), iv);

    StringSource s(message, true,
        new StreamTransformationFilter(e,
            new StringSink(cipher)
        )
    );
    
    std::string ciphertxt;
    std::string ivString;

    //HexEncode IV
    HexEncoder encoder(new FileSink(std::cout));
    encoder.Detach(new StringSink(ivString));
    encoder.Put(iv, iv.size());
    encoder.MessageEnd();

    //HexEncode ciphertxt
    encoder.Detach(new StringSink(ciphertxt));
    encoder.Put((const byte*)&cipher[0], cipher.size());
    encoder.MessageEnd();

    return ivString + "::" + ciphertxt;
}

std::string AESCryptoAlgorithm::decrypt(std::string message)
{
    using namespace CryptoPP;

    size_t found = message.find("::");
    if (found != std::string::npos) {

        std::string ivStr = message.substr(0, found);
        std::string encMessageHex = message.substr(found + 2);

        std::string iv, encMessage;
        HexDecoder decoder, msgDecoder;

        //decode the HEX IV
        decoder.Attach(new StringSink(iv));
        decoder.Put((byte*)ivStr.data(), ivStr.size());
        decoder.MessageEnd();

        //decode the HEX ciphertxt
        decoder.Attach(new StringSink(encMessage));
        decoder.Put((byte*)encMessageHex.data(), encMessageHex.size());
        decoder.MessageEnd();

        
        CBC_Mode< AES >::Decryption d;
        d.SetKeyWithIV(m_key.data(), m_key.size(), (byte*)iv.data(), AES::BLOCKSIZE);

        std::string recovered;
        StringSource s(encMessage, true,
            new StreamTransformationFilter(d,
                new StringSink(recovered)
            )
        );
        return recovered;
    }
    return "{}";
}

std::string AESCryptoAlgorithm::getPublicKey() const
{
    std::string encoded;
    CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(encoded));
    encoder.Put(this->m_key, this->m_key.size());
    encoder.MessageEnd();
    return encoded;
}
