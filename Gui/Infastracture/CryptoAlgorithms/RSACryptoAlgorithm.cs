using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Security.Cryptography;
using System.Security.Cryptography.Xml;
using System.Windows;
using System.Reflection.Metadata;
using System.Security.Cryptography.X509Certificates;
using System.IO;
using Org.BouncyCastle.Crypto;
using Org.BouncyCastle.Crypto.Parameters;
using Org.BouncyCastle.Security;
using Org.BouncyCastle.Utilities;
using Org.BouncyCastle.X509;
using Org.BouncyCastle.Pkcs;

namespace Gui.Infastracture.CryptoAlgorithms
{
    internal class RSACryptoAlgorithm : ICryptoAlgorithm
    {
        RSACryptoServiceProvider m_csp;

        RSAParameters m_privateKey;

        RSAParameters m_publicKey;

        public RSACryptoAlgorithm() 
        {
            m_csp = new RSACryptoServiceProvider(2048);

            m_privateKey = m_csp.ExportParameters(true);

            m_publicKey = m_csp.ExportParameters(true);

        }

        public string getPublicKey()
        {
            byte[] key = m_csp.ExportRSAPublicKey();
            return Convert.ToBase64String(key);
        }

        public string encrypt(string message)
        {
            return this.encrypt(message, this.getPublicKey());
        }

        public string encrypt(string message, string publicKey)
        {
            
            byte[] publicKeyBytes = Convert.FromBase64String(publicKey);

            var rsa = new RSACryptoServiceProvider();
            rsa.ImportRSAPublicKey(publicKeyBytes, out _);
            var rsaParams = rsa.ExportParameters(false);

            return EncryptTextWithRSA(message, rsaParams);
        }
    
        public string decrypt(string message)
        {
            // decrypting data with private key                    

            var resultBytes = Convert.FromBase64String(message);
            var decryptedBytes = m_csp.Decrypt(resultBytes, true);
            var decryptedData = Encoding.UTF8.GetString(decryptedBytes);
            return decryptedData.ToString();
        }

        public static string EncryptTextWithRSA(string text, RSAParameters rsaParameters)
        {
            using (RSACryptoServiceProvider rsa = new RSACryptoServiceProvider())
            {
                rsa.ImportParameters(rsaParameters);
                byte[] textBytes = Encoding.UTF8.GetBytes(text);
                byte[] encryptedBytes = rsa.Encrypt(textBytes, true); // Use false to use PKCS#1 v1.5 padding
                return Convert.ToBase64String(encryptedBytes);
            }
        }
    }
}
