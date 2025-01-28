using System.Text;
using System.Text.Json;

namespace Gui.Infastracture
{
    enum CodeID
    {
        Error,
        Login,
        Signup,
        SignOut,
        GetRooms,
        JoinRoom,
        CreateRoom,
        GetPlayersInRoom,
        GetHighScore,
        GetPersonalStats,
        CloseRoomRequest,
        StartGameRequest,
        GetRoomStateRequest,
        LeaveRoom,
        LeaveGame,
        GetGameState,
        AddQuestionRequest,
        AddUserToOneVOneWait,
        LeaveOneVOneWait,
        GetOneVOneState,
    }

    public interface IRequest
    {}

    public class LoginRequest : IRequest
    {
        public string? username { get; set; }
        public string? password { get; set; }

        public LoginRequest(string username, string password)
        {
            this.username = username;
            this.password = password;
        }
    }

    public class Address
    {
        public string? street { get; set; }
        public int? apt { get; set; } 
        public string? city { get; set; }

        public Address(string? Street, int? Apt, string? City)
        {
            street = Street;
            apt = Apt;
            city = City;
        }
    }

    public class SignupRequest : IRequest
    {
        public string? username { get; set; }
        public string? password { get; set; }
        public string? email { get; set; }
        public Address? addr { get; set; }
        public string? birth { get; set; }
        public string? phone { get; set; }

        public SignupRequest(string? Username, string? Password, string? Email, Address? Addr, string? Birth, string? Phone)
        {
            username = Username;
            password = Password;
            email = Email;
            addr = Addr;
            birth = Birth;
            phone = Phone;
        }
    }

    public class CreateRoomRequest : IRequest
    {
        public string? name { get; set; }
        public int? maxUsers { get; set; }
        public int? questionCount { get; set; }
        public int? answerTimeout { get; set; }


        public CreateRoomRequest(string? roomName, int? maxUsers, int? questionCount, int? answerTimeout)
        {
            this.name = roomName;
            this.maxUsers = maxUsers;
            this.questionCount = questionCount;
            this.answerTimeout = answerTimeout;
        }
    }

    public class GetPlayersRequest : IRequest
    {
        public uint? roomId { get; set; }

        public GetPlayersRequest(uint? roomId) {  this.roomId = roomId; }
    }

    public class JoinRoomRequest : IRequest
    {
        public uint? roomId { get; set; }

        public JoinRoomRequest(uint? roomId) { this.roomId = roomId; }
    }

    public class GetGameStateRequest : IRequest
    {
        public uint? answerId { get; set; }
        public GetGameStateRequest(uint? answerId) { this.answerId = answerId; }
    }

    public class AddNewQuestionRequest : IRequest
    {
        public string? question { get; set; }
        public string? correctAns { get; set; }
        public string? wrongAns1 { get; set; }
        public string? wrongAns2 { get; set; }
        public string? wrongAns3 { get; set; }
        public string? difficulty { get; set; }
        public string? category { get; set; }

        public AddNewQuestionRequest(string? question, string? correctAns, string? wrongAns1, string? wrongAns2, string? wrongAns3, string? difficulty, string? category)
        {
            this.question = question;
            this.correctAns = correctAns;
            this.wrongAns1 = wrongAns1;
            this.wrongAns2 = wrongAns2;
            this.wrongAns3 = wrongAns3;
            this.difficulty = difficulty;
            this.category = category;
        }
    }

    static class Serializer
    {
        public static Byte[] SerializeRequest(LoginRequest loginRequest)
        {
            return SerializeAllRequests(0x01, JsonSerializer.Serialize(loginRequest));
        }

        public static Byte[] SerializeRequest(SignupRequest signupRequest)
        {
            return SerializeAllRequests(0x02, JsonSerializer.Serialize(signupRequest));
        }

        public static Byte[] SerializeRequest(CreateRoomRequest createRoomRequest)
        {
            return SerializeAllRequests(0x06, JsonSerializer.Serialize(createRoomRequest));
        }

        public static Byte[] SerializeRequest(GetPlayersRequest getPlayers)
        {
            return SerializeAllRequests(0x07, JsonSerializer.Serialize(getPlayers));
        }

        public static Byte[] SerializeRequest(JoinRoomRequest joinRoomRequest)
        {
            return SerializeAllRequests((Byte)CodeID.JoinRoom, JsonSerializer.Serialize(joinRoomRequest));
        }

        public static Byte[] SerializeRequest(GetGameStateRequest getGameStateRequest)
        {
            return SerializeAllRequests((Byte)CodeID.GetGameState, JsonSerializer.Serialize(getGameStateRequest));
        }
        public static Byte[] SerializeRequest(AddNewQuestionRequest addNewQuestionRequest)
        {
            return SerializeAllRequests((Byte)CodeID.AddQuestionRequest, JsonSerializer.Serialize(addNewQuestionRequest));
        }

        public static Byte[] SerializeAllRequests(Byte messageCode, string json) 
        {
            byte[] messageCodeBytes = { messageCode };
            
            byte[] jsonBytes = Encoding.UTF8.GetBytes(json);

            byte[] jsonLengthBytes = BitConverter.GetBytes(jsonBytes.Length);
            if (BitConverter.IsLittleEndian)
                Array.Reverse(jsonLengthBytes);

            byte[] serializedRequest = new byte[1 + 4 + jsonBytes.Length];
            Buffer.BlockCopy(messageCodeBytes, 0, serializedRequest, 0, 1);
            Buffer.BlockCopy(jsonLengthBytes, 0, serializedRequest, 1, 4);
            Buffer.BlockCopy(jsonBytes, 0, serializedRequest, 5, jsonBytes.Length);

            return serializedRequest;
        }

        public static Byte[] getPersonalStatsRequest()
        {
            return SerializeAllRequests((int)CodeID.GetPersonalStats, "");
        }

        public static Byte[] getHighScoresRequest()
        {
            return SerializeAllRequests((int)CodeID.GetHighScore, "");
        }

        public static byte[] serializeCipher(string  cipher)
        {
            byte[] cipherBytes = Encoding.UTF8.GetBytes(cipher);

            byte[] cipherLengthBytes = BitConverter.GetBytes(cipherBytes.Length);
            if (BitConverter.IsLittleEndian)
                Array.Reverse(cipherLengthBytes);

            byte[] serializedRequest = new byte[4 + cipherBytes.Length];
            Buffer.BlockCopy(cipherLengthBytes, 0, serializedRequest, 0, 4);
            Buffer.BlockCopy(cipherBytes, 0, serializedRequest, 4, cipherBytes.Length);

            return serializedRequest;
        }
    }
}
