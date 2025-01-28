using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;

namespace Gui.Infastracture.CryptoAlgorithms
{
    internal class AESCryptoAlgorithm : ICryptoAlgorithm
    {

        byte[] key;

        public AESCryptoAlgorithm() 
        {
            using (RijndaelManaged myRijndael = new RijndaelManaged())
            {
                myRijndael.GenerateKey();
                this.key = myRijndael.Key;
            }
        }

        public string encrypt(string message)
        {
            return this.encrypt(message, this.getPublicKey());
        }
        public string encrypt(string message, string publicKey)
        {
            byte[] encrypted;
            byte[] iv;

            using (RijndaelManaged rijAlg = new RijndaelManaged())
            {
                rijAlg.Key = StringToByteArray(publicKey);
                rijAlg.GenerateIV();

                iv = rijAlg.IV;

                // Create a decryptor to perform the stream transform.
                ICryptoTransform encryptor = rijAlg.CreateEncryptor(rijAlg.Key, rijAlg.IV);

                // Create the streams used for encryption. 
                using (MemoryStream msEncrypt = new MemoryStream())
                {
                    using (CryptoStream csEncrypt = new CryptoStream(msEncrypt, encryptor, CryptoStreamMode.Write))
                    {
                        using (StreamWriter swEncrypt = new StreamWriter(csEncrypt))
                        {

                            //Write all data to the stream.
                            swEncrypt.Write(message);
                        }
                        encrypted = msEncrypt.ToArray();
                    }
                }
            }

            // Return the encrypted bytes from the memory stream. 
            return ByteArrayToString(iv) + "::" + ByteArrayToString(encrypted);

        }
        public string decrypt(string message)
        {
            string plaintext = null;

            // Create an RijndaelManaged object 
            // with the specified key and IV. 
            using (RijndaelManaged rijAlg = new RijndaelManaged())
            {
                rijAlg.IV = StringToByteArray(message[..message.IndexOf(':')]);

                // Create a decrytor to perform the stream transform.
                byte[] k = this.key;

                ICryptoTransform decryptor = rijAlg.CreateDecryptor(k, rijAlg.IV);

                // Create the streams used for decryption. 
                using (MemoryStream msDecrypt = new MemoryStream(StringToByteArray(message[(message.IndexOf(':')+2)..])))
                {
                    using (CryptoStream csDecrypt = new CryptoStream(msDecrypt, decryptor, CryptoStreamMode.Read))
                    {
                        using (StreamReader srDecrypt = new StreamReader(csDecrypt))
                        {

                            // Read the decrypted bytes from the decrypting stream 
                            // and place them in a string.
                            plaintext = srDecrypt.ReadToEnd();
                        }
                    }
                }

            }

            return plaintext;
        }
        public string getPublicKey()
        {
            return ByteArrayToString(this.key);
        }
        public static byte[] StringToByteArray(string hex)
        {
            return Enumerable.Range(0, hex.Length)
                             .Where(x => x % 2 == 0)
                             .Select(x => Convert.ToByte(hex.Substring(x, 2), 16))
                             .ToArray();
        }
        public static string ByteArrayToString(byte[] ba)
        {
            return BitConverter.ToString(ba).Replace("-", "");
        }

    }
}
