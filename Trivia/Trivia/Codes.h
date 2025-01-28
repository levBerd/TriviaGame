#pragma once

const enum class CodeId {
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
	AddNewQuestion,
	AddUserToOneVOneWait,
	LeaveOneVOneWait,
	GetOneVOneState,
	Amount = 0x50
};

const enum class stutusId {
	Failed,
	Success,
	IncorrectAnswer
};