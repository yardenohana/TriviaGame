#include "Question.h"

Question::Question(std::string question, std::string answer1, std::string answer2, std::string answer3, std::string answer4)
	: question(question), answer1(answer1), answer2(answer2), answer3(answer3), answer4(answer4)
{
}

void Question::setQuestion(std::string question)
{
	this->question = question;
}

void Question::setAnswer1(std::string answer)
{
	this->answer1 = answer;
}

void Question::setAnswer2(std::string answer)
{
	this->answer2 = answer;
}

void Question::setAnswer3(std::string answer)
{
	this->answer3 = answer;
}

void Question::setAnswer4(std::string answer)
{
	this->answer4 = answer;
}

std::string Question::getQuestion() const
{
	return this->question;
}

std::string Question::getAnswer1() const
{
	return this->answer1;
}

std::string Question::getAnswer2() const
{
	return  this->answer2;
}

std::string Question::getAnswer3() const
{
	return this->answer3;
}

std::string Question::getAnswer4() const
{
	return answer4;
}
