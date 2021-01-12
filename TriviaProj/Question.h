#pragma once
#include <iostream>

class Question {

private:
	std::string question;

	//answer1 is alwayas the correct one.
	std::string answer1;
	std::string answer2;
	std::string answer3;
	std::string answer4;

public:
	Question(std::string question, std::string answer1, std::string answer2, 
		     std::string answer3, std::string answer4);
	Question() = default;
	~Question() = default;

	void setQuestion(std::string question);
	void setAnswer1(std::string answer);
	void setAnswer2(std::string answer);
	void setAnswer3(std::string answer);
	void setAnswer4(std::string answer);

	std::string getQuestion() const;
	std::string getAnswer1() const;
	std::string getAnswer2() const;
	std::string getAnswer3() const;
	std::string getAnswer4() const;
};
