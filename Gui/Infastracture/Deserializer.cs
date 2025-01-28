using System.Text.Json;

namespace Gui.Infastracture
{
    enum stutusId
    {
        Failed,
	    Success
    };

    public class PersonalStatsResponse
    {
        public uint? status { get; set; }
        public List<string>? statistics { get; set; }
    }

    public class LoginResponse
    {
        public uint? status { get; set; }
    }

    public class SignupResponse
    {
        public uint? status { get; set; }
    }

    public class Room
    {
        public uint id { get; set; }
        public string name { get; set; }
        public int maxPlayers { get; set; }
        public int numOfQuestionsInGame { get; set; }
        public int timePerQuestion { get; set; }
        public uint status { get; set; }
    }
    public class GetRoomsResponse
    {
        public uint? status { get; set; }
        public List<Room> rooms { get; set; }
    }

    public class CreateRoomResponse
    {
        public uint? status { get; set; }
    }

    public class ErrorResponse
    {
        public string? message { get; set; }
    }

    public class HighScoreResponse
    {
        public uint? status { get; set; }
        public List<string>? statistics { get; set; }
    }

    public class PlayersInRoomResponse
    {
        public uint? status { get; set; }
        public List<string>? players { get; set; }
    }

    public class GetRoomStateResponse
    {
        public uint? status { get; set; }
        public uint? roomState { get; set; }
        public List<string>? players { get; set; }
        public uint? questionCount { get; set; }
        public uint? answerTimeOut { get; set; }
    }

    public class JoinRoomResponse
    {
        public uint? status { get; set; }
    }

    public class LeaveRoomResponse
    {
        public uint? status { get; set; }
    }

    public class CloseRoomResponse
    {
        public uint? status { get; set; }
    }

    public class StartGameResponse
    {
        public uint? status { get; set; }
    }

    public class PlayerResult
    {
        public string username { get; set; }
        public int correctAnswerCount { get; set; }
        public int wrongAnswerCount { get; set; }
        public float averageAnswerTime { get; set; }
    }

    public class GameStateResponse
    {
        public uint status { get; set; }
        public uint correctAnswerId { get; set; }
        public uint timeLeft { get; set; }
        public string question { get; set; }
        public Dictionary<int, string> answers { get; set; }
        public List<PlayerResult> results { get; set; }
        public List<string> playerNames { get; set; }
    }

    public class LeaveGameResponse
    {
        public uint? status { get; set; }
    }

    public class AddNewQuestionResponse
    {
        public uint? status { get; set; }
    }
    public class AddUserToOneVOneWaitResponse
    {
        public uint? status { get; set; }
        public uint? state { get; set; }
    }
    public class LeaveOneVOneWaitResponse
    {
        public uint? status { get; set; }
    }
    public class GetOneVOneStateResponse
    {
        public uint? status { get; set; }
        public uint? roomState { get; set; }
        public string opponentName { get; set; }
    }
    static class Deserializer
    {
        public static T? DeserializeResponse<T>(byte[] buffer)
        {
            string json = System.Text.Encoding.ASCII.GetString(buffer[5..]);
            // string trimmedJson = json.Substring(0, (json.IndexOf('\x00')));
            return JsonSerializer.Deserialize<T>(json);
        }
    }
}
