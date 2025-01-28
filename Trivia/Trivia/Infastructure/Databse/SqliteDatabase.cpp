#include "SqliteDatabase.h"
#include <iostream>

struct GameForEachPlayer {
    std::string playerName;
    int questionAnswerd;
    int questionAnswerdCorrect;
    float averageTimeToAnswer;
};


int answer_vector_callback(void* data, int argc, char** argv, char** azColName) {
    std::vector<Answer>* answers = (std::vector<Answer>*)data;
    Answer a;
    a.question_id = std::atoi(argv[1]);
    a.answer = argv[2];
    a.id = std::atoi(argv[0]);
    a.isCorrect = std::atoi(argv[3]) == 1;

    answers->push_back(a);
    return 0;
}

int player_vector_callback(void* data, int argc, char** argv, char** azColName) {
    std::vector<std::string>* players = (std::vector<std::string>*)data;

    players->push_back(argv[1]);
    return 0;
}

int questions_list_callback(void* data, int argc, char** argv, char** azColName) {
    std::list<std::pair<int, std::string>>* p = (std::list<std::pair<int, std::string>>*)data;

    p->push_back({std::atoi(argv[0]), argv[1]});
    return 0;
}

int game_vect_callback(void* data, int argc, char** argv, char** azColName) {
    std::vector<GameForEachPlayer>* p = (std::vector<GameForEachPlayer>*)data;

    GameForEachPlayer game;
    game.playerName = argv[1];
    game.questionAnswerd = std::atoi(argv[2]);
    game.questionAnswerdCorrect = std::atoi(argv[3]);
    game.averageTimeToAnswer = std::stof(argv[4]);

    p->push_back(game);
    return 0;
}

bool SqliteDatabase::open()
{
    int doesFileExist = _access(this->dbFileName.c_str(), 0);
    int res = sqlite3_open(this->dbFileName.c_str(), &this->db);
    if (res != SQLITE_OK) {
        db = nullptr;
        std::cout << "Failed to open DB" << std::endl;
        return false;
    }

    // Build the tables if the file doesn't exist
    const char* createUserTableSQL = "CREATE TABLE IF NOT EXISTS Users ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
        "NAME TEXT NOT NULL,"
        "PASS TEXT NOT NULL,"
        "EMAIL TEXT NOT NULL,"
        "ADDR TEXT NOT NULL,"
        "BIRTH TEXT NOT NULL,"
        "PHONE TEXT NOT NULL);";


    char* errMessage = nullptr;
    std::unique_lock writeLock(m_mutex);
    res = sqlite3_exec(db, createUserTableSQL, nullptr, nullptr, &errMessage);
    if (res != SQLITE_OK) {
        std::cerr << "Error creating USERS table: " << errMessage << std::endl;
        sqlite3_free(errMessage);
        return false;
    }


    // Build the game table (this will be the statistics table also)
    createUserTableSQL = "CREATE TABLE IF NOT EXISTS GAMES ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
        "USER_NAME TEXT NOT NULL,"
        "QUESTIONS_ANSWERS INT NOT NULL,"
        "QUESTIONS_ANSWERS_CORRECT INT NOT NULL,"
        "TOTAL_ANSWER_TIME REAL NOT NULL,"
        "FOREIGN KEY(USER_NAME) REFERENCES USERS(NAME)); ";


    errMessage = nullptr;
    res = sqlite3_exec(db, createUserTableSQL, nullptr, nullptr, &errMessage);
    if (res != SQLITE_OK) {
        std::cerr << "Error creating GAEMS table: " << errMessage << std::endl;
        sqlite3_free(errMessage);
        return false;
    }

    return true;

}

bool SqliteDatabase::close()
{
    if (sqlite3_close(this->db) == SQLITE_OK) {
        this->db = nullptr;
        return true;
    }
    else {
        return false;
    }
}

bool SqliteDatabase::doesUserExists(const std::string& username)
{
    const char* query = "SELECT COUNT(*) FROM USERS WHERE NAME = ?";
    sqlite3_stmt* stmt;

    std::shared_lock readLock(m_mutex);
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        throw ManagerException("Error: Failed to prepare statement. " + std::string(sqlite3_errmsg(db)));
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return count > 0;
    }

    sqlite3_finalize(stmt);
    int rowsAffected = sqlite3_changes(db);
    rowsAffected += sqlite3_changes(this->db);
    return rowsAffected>0;

}

bool SqliteDatabase::doesPasswordMatch(const std::string& username, const std::string& password)
{
    const char* query = "SELECT PASS FROM USERS WHERE NAME = ?";
    sqlite3_stmt* stmt;

    std::shared_lock readLock(m_mutex);
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        throw ManagerException("Error: Failed to prepare statement. " + std::string(sqlite3_errmsg(db)));
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* dbPassword = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        bool isMatch = password == std::string(dbPassword);
        sqlite3_finalize(stmt);
        return isMatch;
    }

    sqlite3_finalize(stmt);
    return false;
}

bool SqliteDatabase::addNewUser(const std::string& username, const std::string& password, const std::string& email, const std::string& addr, const std::string& birth, const std::string& phone)
{
    std::string query = "INSERT INTO USERS (NAME, PASS, EMAIL, ADDR, BIRTH, PHONE) VALUES (?, ?, ?, ?, ?, ?)";
    sqlite3_stmt* stmt;

    std::unique_lock writeLock(m_mutex);
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw ManagerException("Error: Failed to prepare statement. " + std::string(sqlite3_errmsg(db)));
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, addr.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, birth.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, phone.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw ManagerException("Error: Failed to prepare statement. " + std::string(sqlite3_errmsg(db)));
    }

    int rowsAffected = sqlite3_changes(db);
    sqlite3_finalize(stmt);

    return rowsAffected;
}

std::vector<Question> SqliteDatabase::getQuestions(const int n)
{
    std::list<std::pair<int, std::string>> questions_pairs;

    std::string sqlStatement = "SELECT * FROM QUESTIONS WHERE ID IN (SELECT ID FROM QUESTIONS ORDER BY RANDOM() LIMIT " + std::to_string(n) + ");";
    char* errMessage = nullptr;
    std::shared_lock readLock(m_mutex);
    int res = sqlite3_exec(this->db, sqlStatement.c_str(), questions_list_callback, &questions_pairs, &errMessage);
    readLock.unlock();
    if (res != SQLITE_OK) {
        throw ManagerException("Error: couldnt find questions: " + std::string(errMessage));
    }

    std::vector<Question> questions;

    for (std::pair<int, std::string> p : questions_pairs) {
        std::vector<Answer> answers = this->getAnswersOfQuestionId(p.first);
        questions.push_back(Question (p.second, answers));
    }

    return questions;
}

float SqliteDatabase::getPlayerAverageAnswerTime(const std::string& playerName)
{
    const char* query = "SELECT avg(TOTAL_ANSWER_TIME) FROM GAMES WHERE USER_NAME = ?";
    sqlite3_stmt* stmt;
    std::shared_lock readLock(m_mutex);
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        throw ManagerException("Error: Failed to prepare statement. " + std::string(sqlite3_errmsg(db)));
    }

    sqlite3_bind_text(stmt, 1, playerName.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        return sqlite3_column_double(stmt, 0);
    }

    throw ManagerException("Error: failed to find user: " + std::string(sqlite3_errmsg(db)));
}

int SqliteDatabase::getNumOfCorrectAnswers(const std::string& playerName)
{
    const char* query = "SELECT SUM(QUESTIONS_ANSWERS_CORRECT) FROM GAMES WHERE USER_NAME = ?";
    sqlite3_stmt* stmt;

    std::shared_lock readLock(m_mutex);
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        throw ManagerException("Error: Failed to prepare statement. " + std::string(sqlite3_errmsg(db)));
    }

    sqlite3_bind_text(stmt, 1, playerName.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        return sqlite3_column_int(stmt, 0);
    }

    throw ManagerException("Error: failed to find user: " + std::string(sqlite3_errmsg(db)));
}

int SqliteDatabase::getNumOfTotalAnswers(const std::string& playerName)
{
    const char* query = "SELECT SUM(QUESTIONS_ANSWERS) FROM GAMES WHERE USER_NAME = ?";
    sqlite3_stmt* stmt;

    std::shared_lock readLock(m_mutex);
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        throw ManagerException("Error: Failed to prepare statement. " + std::string(sqlite3_errmsg(db)));
    }

    sqlite3_bind_text(stmt, 1, playerName.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        return sqlite3_column_int(stmt, 0);
    }

    throw ManagerException("Error: failed to find user: " + std::string(sqlite3_errmsg(db)));
}

int SqliteDatabase::getNumOfPlayerGames(const std::string& playerName)
{
    std::string sqlStatement = "SELECT * FROM GAMES WHERE USER_NAME == '" + playerName + "';";

    std::vector<GameForEachPlayer> games;

    char* errMessage = nullptr;
    std::shared_lock readLock(m_mutex);
    int res = sqlite3_exec(this->db, sqlStatement.c_str(), answer_vector_callback, &games, &errMessage);
    readLock.unlock();
    if (res != SQLITE_OK) {
        throw ManagerException("Error: couldnt find questions: " + std::string(errMessage));
    }

    return games.size();
}

int SqliteDatabase::getPlayerScore(const std::string& playerName)
{
    // score is calculated like this: Score=(Correct Answer PointsxAccuracy)+(Participation PointsxParticipation Factor)−(Speed PenaltyxSpeed Factor)
    float correctAnswerPoints = 10;
    float participationPoints = 3;
    float speedPenalty = 0.2f;

    int correcAnswers = this->getNumOfCorrectAnswers(playerName);

    float accurecy = 0;
    if (correcAnswers != 0) {
        accurecy = (float)this->getNumOfTotalAnswers(playerName) / (float)correcAnswers;
    }

    float participationFactor = (float)this->getNumOfPlayerGames(playerName);
    float speedFactor = this->getPlayerAverageAnswerTime(playerName);

    return (int)((accurecy * correctAnswerPoints) + (participationFactor * participationPoints) - (speedFactor * speedPenalty));
}

std::vector<std::string> SqliteDatabase::getHighScores()
{
    std::vector <std::pair<std::string, int>> player_scores;

    std::vector<std::string> players = this->getPlayers();
    for (std::string player : players) {
        player_scores.push_back(std::pair<std::string, int>( player, this->getPlayerScore(player)));
    }

    std::sort(player_scores.begin(), player_scores.end(), [](auto& left, auto& right) { return left.second > right.second; });

    std::vector<std::string> highScores;
    
    int playerCount = 0;
    for (const std::pair<std::string, int>& p: player_scores){
        if (playerCount >= 5){
            break;
        }

        highScores.push_back(p.first + ": " + std::to_string(p.second));

        playerCount++;
    }

    return highScores;

}

int SqliteDatabase::submitGameStatistics(const std::pair<LoggedUser, UserData>& gameData)
{
    std::string query = "INSERT INTO GAMES (USER_NAME, QUESTIONS_ANSWERS, QUESTIONS_ANSWERS_CORRECT, TOTAL_ANSWER_TIME) VALUES ('" + gameData.first.getName() + "', ?, ?, ?)";
    sqlite3_stmt* stmt;
    std::unique_lock writeLock(m_mutex);
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw ManagerException("Error: Failed to prepare statement. " + std::string(sqlite3_errmsg(db)));
    }

    sqlite3_bind_int64(stmt, 1, gameData.second.correctAnswerCount + gameData.second.wrongAnswerCount);
    sqlite3_bind_int64(stmt, 2, gameData.second.correctAnswerCount);
    sqlite3_bind_double(stmt, 3, gameData.second.totalAnswerTime);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw ManagerException("Error: Failed to prepare statement. " + std::string(sqlite3_errmsg(db)));
    }

    int rowsAffected = sqlite3_changes(db);
    sqlite3_finalize(stmt);

    return rowsAffected;
}

void SqliteDatabase::addNewQuestion(std::string question, std::string difficulty, std::string category,
    std::string correctAns, std::string wrongAns1, std::string wrongAns2, std::string wrongAns3)
{
    const char* insertQuestionSql = "INSERT INTO QUESTIONS (QUESTION, DIFFICULTY, CATEGORY) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, insertQuestionSql, -1, &stmt, nullptr);

    if (result != SQLITE_OK) {
        throw ManagerException("Failed to prepare question statement: " + std::string(sqlite3_errmsg(db)));
        return;
    }

    sqlite3_bind_text(stmt, 1, question.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, difficulty.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, category.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);

    if (result != SQLITE_DONE) {
        ManagerException("Failed to execute question statement: " + std::string(sqlite3_errmsg(db)));
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);

    int questionId = static_cast<int>(sqlite3_last_insert_rowid(db));

    const char* insertAnswerSql = "INSERT INTO ANSWERS (QUESTION_ID, ANSWER, IS_CORRECT) VALUES (?, ?, ?);";

    result = sqlite3_prepare_v2(db, insertAnswerSql, -1, &stmt, nullptr);

    if (result != SQLITE_OK) {
        std::cerr << "Failed to prepare answer statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    struct Answer {
        std::string text;
        bool isCorrect;
    };

    Answer answers[] = {
        { correctAns, true },
        { wrongAns1, false },
        { wrongAns2, false },
        { wrongAns3, false }
    };

    for (const auto& answer : answers) {
        sqlite3_bind_int(stmt, 1, questionId);
        sqlite3_bind_text(stmt, 2, answer.text.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, answer.isCorrect);

        result = sqlite3_step(stmt);

        if (result != SQLITE_DONE) {
            std::cerr << "Failed to execute answer statement: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            return;
        }

        sqlite3_reset(stmt);
    }

    sqlite3_finalize(stmt);
}

std::vector<Answer> SqliteDatabase::getAnswersOfQuestionId(const int id) 
{
    std::vector<Answer> answers;

    std::string sqlStatement = "SELECT * FROM ANSWERS WHERE QUESTION_ID == " + std::to_string(id) + " ORDER BY RANDOM();";
    char* errMessage = nullptr;
    std::shared_lock readLock(m_mutex);
    int res = sqlite3_exec(this->db, sqlStatement.c_str(), answer_vector_callback, &answers, &errMessage);
    readLock.unlock();
    if (res != SQLITE_OK) {
        throw ManagerException("Error: couldnt find questions: " + std::string(errMessage));
    }
    
    return answers;
}

std::vector<std::string> SqliteDatabase::getPlayers()
{
    std::vector<std::string> players;

    std::string sqlStatement = "SELECT * FROM USERS";
    char* errMessage = nullptr;
    std::shared_lock readLock(m_mutex);
    int res = sqlite3_exec(this->db, sqlStatement.c_str(), player_vector_callback, &players, &errMessage);
    readLock.unlock();
    if (res != SQLITE_OK) {
        throw ManagerException("Error: couldnt find players: " + std::string(errMessage));
    }

    return players;
}
