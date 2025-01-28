using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Gui.Infastracture.CryptoAlgorithms
{
    internal class OTPCryptoAlgorithm : ICryptoAlgorithm
    {
        public string getPublicKey()
        {
            return "";
        }

        public string encrypt(string message, string publicKey)
        {
            return this.encrypt(message);
        }

        public string decrypt(string message)
        {
            String key = message.Substring(message.Length / 2);
            String cipher = message.Substring(0, message.Length / 2);
            String decryptedMessage = "";

            for (int i = 0; i < cipher.Length; i++)
            {
                if (cipher[i] >= 'A' && cipher[i] <= 'Z')
                {
                    if ((message[i] - 'A' - key[i] + 'a') < 0)
                    {
                        decryptedMessage += Convert.ToChar('A' + (message[i] - 'A' - key[i] + 'a') + 26);
                    }
                    else
                    {
                        decryptedMessage += Convert.ToChar('A' + (message[i] - 'A' - key[i] + 'a'));
                    }
                }
                else if (message[i] >= 'a' && message[i] <= 'z')
                {
                    if ((message[i] - 'a' - key[i] + 'a') < 0)
                    {
                        decryptedMessage += Convert.ToChar('a' + (message[i] - 'a' - key[i] + 'a') + 26);
                    }
                    else
                    {
                        decryptedMessage += Convert.ToChar('a' + (message[i] - 'a' - key[i] + 'a'));
                    }
                }
                else
                {
                    decryptedMessage += Convert.ToChar(message[i]);
                }
            }

            return decryptedMessage;
        }

        public string encrypt(string message)
        {
            String key = generateKey(message);
            String encryptedMessage = "";

            for (int i = 0; i < message.Length ; i++)
            {
                if (message[i] >= 'A' && message[i] <= 'Z')
                {
                    encryptedMessage += (char)('A' + ((message[i] - 'A' + key[i] - 'a') % 26));
                }
                else if (message[i] >= 'a' && message[i] <= 'z')
                {
                    encryptedMessage += (char)('a' + ((message[i] - 'a' + key[i] - 'a') % 26));
                }
                else
                {
                    encryptedMessage += message[i];
                }
            }

            return encryptedMessage + key;
        }

        static public string generateKey(string message)
        {
            Random random = new Random();
            const string chars = "abcdefghijklmnopqrstuvwxyz";
            return new string(Enumerable.Repeat(chars, message.Length)
                .Select(s => s[random.Next(s.Length)]).ToArray());
        }
    }
}
