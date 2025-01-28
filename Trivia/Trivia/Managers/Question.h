#pragma once
#include <vector>
#include <string>
#include <map>

struct Answer {
	std::string answer;
	int id;
	int question_id;
	bool isCorrect;
};


class Question
{
public:
	Question(std::string question, std::vector<Answer> answers);
	~Question() = default;

	std::string getQuestion() const;
	std::vector<std::string> getAnswers() const;
	std::map<int, std::string> getAnswersMap() const;
	int getCorrectAnswerId() const;

	Question& operator= (const Question& other);


private:
	std::vector<Answer> m_possibleAnswers;
	std::string m_question;
};

