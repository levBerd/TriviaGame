#include "Question.h"
#include "ManagerException.h"


Question::Question(std::string question, std::vector<Answer> answers) : m_possibleAnswers(answers), m_question(question)
{
}

std::string Question::getQuestion() const
{
	return this->m_question;
}

std::vector<std::string> Question::getAnswers() const
{
	std::vector<std::string> answers;

	for (Answer ans : this->m_possibleAnswers) {
		answers.push_back(ans.answer);
	}

	return answers;
}

std::map<int, std::string> Question::getAnswersMap() const
{
	std::map<int, std::string> answers;

	for (Answer answer : this->m_possibleAnswers) {
		answers.insert({answer.id, answer.answer});
	}

	return answers;
}

int Question::getCorrectAnswerId() const
{
	for (Answer ans : this->m_possibleAnswers) {
		if (ans.isCorrect) {
			return ans.id;
		}
	}
	throw ManagerException("No correct answer for some reason :/");
	return -1;
}

Question& Question::operator=(const Question& other)
{
	m_possibleAnswers = other.m_possibleAnswers;
	m_question = other.m_question;

	return *this;
}
