using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gui.Infastracture.CryptoAlgorithms
{
    internal interface ICryptoAlgorithm
    {
        abstract string encrypt(string message);
        abstract string encrypt(string message, string publicKey);
        abstract string decrypt(string message);
        abstract string getPublicKey();
    }
}
