using System.Net.Sockets;
using System.Net;
using System.Windows;
using System.Text;
using Gui.Infastracture;
using Gui.Infastracture.CryptoAlgorithms;

namespace Gui
{
    public class Communicator
    {
        private readonly Socket? m_socket;
        string m_server_public_key;
        ICryptoAlgorithm m_cryptoAlgorithm;
        public bool IsConnected { get; private set; }

        public Communicator(string IP)
        {
            try
            {
                // create cryptoAlgorithm
                m_cryptoAlgorithm = new AESCryptoAlgorithm();

                IPAddress ipAddress = IPAddress.Parse(IP);
                IPEndPoint localEndPoint = new IPEndPoint(ipAddress, 8826);

                // connect to server
                m_socket = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
                m_socket.Connect(localEndPoint);


                // send and receive public key
                Byte[] server_key_array = sendMessageNoEncryption(Encoding.ASCII.GetBytes(m_cryptoAlgorithm.getPublicKey()));
                m_server_public_key = Encoding.ASCII.GetString(server_key_array);
                m_server_public_key = m_server_public_key[..m_server_public_key.IndexOf('\x00')];
                IsConnected = true;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Socket Error", MessageBoxButton.OK, MessageBoxImage.Error);
                IsConnected = false;
            }
        }

        public void CloseConnection()
        {
            m_socket?.Shutdown(SocketShutdown.Both);
            m_socket?.Close();
        }

        public byte[] sendMessageNoEncryption(byte[] message)
        {
            m_socket?.Send(Serializer.serializeCipher(Encoding.ASCII.GetString(message)));

            byte[] response = new byte[1024];
            m_socket?.Receive(response);

            return response;
        }

        public byte[] sendMessage(Byte[] request)
        {
            string cipher = m_cryptoAlgorithm.encrypt(Encoding.ASCII.GetString(request), m_server_public_key);

            m_socket?.Send(Serializer.serializeCipher(cipher));

            byte[] response = new byte[1024];
            m_socket?.Receive(response);

            string encryptedData = Encoding.ASCII.GetString(response[4..]);
            encryptedData = encryptedData.Substring(0, encryptedData.IndexOf('\x00'));

            string recovered = m_cryptoAlgorithm.decrypt(encryptedData);

            return Encoding.ASCII.GetBytes(recovered);
        }
    }    
}
