#include "SqliteDatabase.h"

/*
The function converts c string to std::string type.
Input: the c-string 
Output: the std::string 
*/
std::string SqliteDatabase::charPtrToString(const char* str)
{
	std::string result = "";
	for (int i = 0; str[i] != NULL; i++)
		result += str[i];
	return result;
}

int SqliteDatabase::callback(void* NotUsed, int argc, char** argv, char** azColName)
{
	//argc : number of colums
	// argv: the data of the record.
	//for the work with sqlite
	int i;
	for (i = 0; i < argc; i++)
	{
		std::cout << argv[i] << "\t";
	}
	printf("\n");
	return 0;
}

/*
The function initializes the database.
Input: none
Output: true if succeded, else false.
*/
bool SqliteDatabase::initDatabase() const
{
	std::cout << "[+] Initializing db!" << std::endl;
	// init database:
	//sending a queries to the SQL server, to create the 4 tables. 
	//if there is an error during the proccess, an exception will be thrown.
	std::string UsersTable = "CREATE TABLE USERS(" \
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "\
		"NAME TEXT NOT NULL, "\
		"PASSWORD TEXT NOT NULL, "\
		"EMAIL TEXT NOT NULL"\
		");";

	char* zErrMsg;
	int res = sqlite3_exec(m_database, UsersTable.c_str(), callback, 0, &zErrMsg);
	if (res != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return false;
	}

	// Answer1 will be the right answer
	std::string QuestionsTable = "CREATE TABLE QUESTIONS(" \
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "\
		"QUESTION TEXT NOT NULL, "\
		"ANSWER1 TEXT NOT NULL, "\
		"ANSWER2 TEXT NOT NULL, "\
		"ANSWER3 TEXT NOT NULL, "\
		"ANSWER4 TEXT NOT NULL"\
		");";

	res = sqlite3_exec(m_database, QuestionsTable.c_str(), callback, 0, &zErrMsg);
	if (res != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return false;
	}

	std::string statisticsTable = 
		"CREATE TABLE STATISTICS("\
		"NAME TEXT PRIMARY KEY NOT NULL,"\
		"GAMES integer  NOT NULL DEFAULT 0,"\
		"NUMBER_OF_ANSWERS integer  NOT NULL DEFAULT 0,"\
		"CORRECT_ANSWERS integer NOT NULL DEFAULT 0, "\
		"AVRG_TIME REAL NOT NULL DEFAULT 0.0, "\
		"WINS integer NOT NULL DEFAULT 0, "\
		"CONSTRAINT FK_USERS FOREIGN KEY(NAME) REFERENCES USERS(NAME) ON DELETE CASCADE"\
		");";

	res = sqlite3_exec(m_database, statisticsTable.c_str(), callback, 0, &zErrMsg);
	if (res != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return false;
	}


	return true;
}

/*
A basic callback function, used to 
retrieve data from the sqlite database.
Input: argc- number of colums at table, argv- the records at the table.
*/
int SqliteDatabase::usersCallback(void* NotUsed, int argc, char** argv, char** azColName)
{
	int name = 0;
	
	name = std::stoi(charPtrToString(argv[0]));
	*((int*)(NotUsed)) = name;

	return 0;
}
/*
A basic callback function, used to
retrieve data from the sqlite database.
this function is to receive a specific data type - double.
Input: argc - number of colums at table, argv - the records at the table.
*/
int SqliteDatabase::numCallback(void* NotUsed, int argc, char** argv, char** azColName)
{
	if (argc == 1 && argv) {
		*static_cast<double*>(NotUsed) = atoi(argv[0]);
		return 0;
	}
	return 1;
}

/*
A basic callback function, used to
retrieve data from the sqlite database.
Input: argc- number of colums at table, argv- the records at the table.
*/
int SqliteDatabase::questionsCallback(void* data, int argc, char** argv, char** azColName)
{
	std::list<Question>* listOfQuestions = static_cast<std::list<Question>*>(data);
	
	Question q;
	for (int i = 0; i < argc; i++) 
	{
		if (std::string(azColName[i]) == "QUESTION") {
			q.setQuestion(charPtrToString(argv[i]));
		}
		else if (std::string(azColName[i]) == "ANSWER1") {
			q.setAnswer1(charPtrToString(argv[i]));
		}
		else if (std::string(azColName[i]) == "ANSWER2") {
			q.setAnswer2(charPtrToString(argv[i]));
		}
		else if (std::string(azColName[i]) == "ANSWER3") {
			q.setAnswer3(charPtrToString(argv[i]));
		}
		else if (std::string(azColName[i]) == "ANSWER4") {
			q.setAnswer4(charPtrToString(argv[i]));
		}
	}

	listOfQuestions->push_back(q);
	return 0;
	
}

// Inserts the Trivia questions into the database
bool SqliteDatabase::questions() const
{
	bool res = true;;
	std::string a = "Where were the fortune cookies invented?";
	std::string a1 = "San Francisco";
	std::string a2 = "Shanghai";
	std::string a3 = "New York";
	std::string a4 = "Bangkok";
	res = res && queryQuestion(a, a1, a2, a3, a4);

	std::string b = "What animal cannot stick out their tongue?";
	std::string b1 = "Crocodiles";
	std::string b2 = "Cats";
	std::string b3 = "Lizards";
	std::string b4 = "birds";
	res = res && queryQuestion(b, b1, b2, b3, b4);

	std::string c = "With how many bricks is the Empire State Building is made of?";
	std::string c1 = "10 million";
	std::string c2 = "50 million";
	std::string c3 = "1 billion";
	std::string c4 = "500 thousend";
	res = res && queryQuestion(c, c1, c2, c3, c4);

	std::string d = "What was Walt Disney afraid of?";
	std::string d1 = "Mice";
	std::string d2 = "Clowns";
	std::string d3 = "The Dark";
	std::string d4 = "Spiders";
	res = res && queryQuestion(d, d1, d2, d3, d4);

	std::string e = "Where do kiwi fruits originally come from?";
	std::string e1 = "China";
	std::string e2 = "Australia";
	std::string e3 = "Thailand";
	std::string e4 = "japan";
	res = res && queryQuestion(e, e1, e2, e3, e4);

	std::string f = "What was the first fruit that was eaten on the moon?";
	std::string f1 = "Peach";
	std::string f2 = "Apple";
	std::string f3 = "Pear";
	std::string f4 = "Watermelon";
	res = res && queryQuestion(f, f1, f2, f3, f4);

	std::string g = "What country won the Eurovision Song Contest 2017?";
	std::string g1 = "Portugal";
	std::string g2 = "Israel";
	std::string g3 = "England";
	std::string g4 = "Scotland";
	res = res && queryQuestion(g, g1, g2, g3, g4);

	std::string h = "What color is the black box in an airplane?";
	std::string h1 = "Orange";
	std::string h2 = "Black";
	std::string h3 = "Blue";
	std::string h4 = "Green";
	res = res && queryQuestion(h, h1, h2, h3, h4);

	std::string i = "on FRIENDS, who said to who - we were on a break! - ?";
	std::string i1 = "Ross to Rachel";
	std::string i2 = "Monica to Chandler";
	std::string i3 = "Phobe to Rachel";
	std::string i4 = "Rachel to Gunther";
	res = res && queryQuestion(i, i1, i2, i3, i4);

	std::string j = " Who wrote the Harry Poter books?";
	std::string j1 = "J. K. Rowling";
	std::string j2 = "Rick Riordan";
	std::string j3 = "Veronica Roth";
	std::string j4 = "Suzanne  Collins";
	res = res && queryQuestion(j, j1, j2, j3, j4);
	return res; 

}

/*
function gets question and four answers and inserts them into the db.
returns true if action worked, false if otherwise.
*/
bool SqliteDatabase::queryQuestion(std::string question, std::string a, std::string b, std::string c, std::string d) const
{
	std::string query = "INSERT INTO QUESTIONS (QUESTION, ANSWER1, ANSWER2, ANSWER3, ANSWER4) "\
		"VALUES (";

	query += "'" + question + "', '" + a + "','" + b + "','" + c + "','" + d + "');";
	char* zErrMsg;
	int res = sqlite3_exec(m_database, query.c_str(), nullptr, nullptr, &zErrMsg);

	if (res != SQLITE_OK) // sql error
	{
		sqlite3_free(zErrMsg);
		std::cout << zErrMsg << std::endl;
		return false;
	}

	return true;
}

/*
The function will return the specific number of questions.
Input: number of questions
Output: the list of questions
*/
std::list<Question> SqliteDatabase::getQuestions(int numOfQuestions) const
{

	std::string sqlStatement = "SELECT * FROM QUESTIONS LIMIT " + std::to_string(numOfQuestions) + ";";
	
	std::list<Question> listOfQuestions;
	char* errMessage = nullptr;
	int res = sqlite3_exec(m_database, sqlStatement.c_str(), questionsCallback, &listOfQuestions, &errMessage);
	
	if (res != SQLITE_OK) {
		std::cout << "Error: " << errMessage << std::endl;
		sqlite3_free(errMessage);
	}
	
	return listOfQuestions;
}

/*
The function receives the username of a user and returns the average time it took for them to answer.
the function gets the answer from the DB.
input: the username, string.
output: the average time it took the user to answer, double.
*/
double SqliteDatabase::getPlayerAverageAnswerTime(std::string username) const
{
	std::string query = "SELECT AVRG_TIME FROM STATISTICS WHERE NAME ='" + username + "';";

	double ans = 0; // result of avrg_time query
	char* zErrMsg = 0;

	/* Execute SQL statement */
	int res = sqlite3_exec(m_database, query.c_str(), numCallback, &ans, &zErrMsg);

	if (res != SQLITE_OK) // sql error
	{
		std::cout << "Error: " << zErrMsg << std::endl;
		sqlite3_free(zErrMsg);
		return false;
	}

	return ans;
}

/*
The function receives the username of a user and returns the number of correct answers they have.
the function gets the answer from the DB.
input: the username, string.
output: the number of correct answer the user had, int.
*/
int SqliteDatabase::getNumOfCorrectAnswers(std::string username) const
{
	std::string query = "SELECT CORRECT_ANSWERS FROM STATISTICS WHERE NAME ='" + username + "';";

	double ans = 0; // result of correct_answers query
	char* zErrMsg = 0;

	/* Execute SQL statement */
	int res = sqlite3_exec(m_database, query.c_str(), numCallback, &ans, &zErrMsg);

	if (res != SQLITE_OK) // sql error
	{
		std::cout << "Error: " << zErrMsg << std::endl;
		sqlite3_free(zErrMsg);
		return false;
	}

	return (int)ans;
}

/*
The function receives the username of a user and returns the number of answers they have.
the function gets the answer from the DB.
input: the username, string.
output: the number of answer the user had, int.
*/
int SqliteDatabase::getNumOfTotalAnswers(std::string username) const
{
	std::string query = "SELECT NUMBER_OF_ANSWERS FROM STATISTICS WHERE NAME ='" + username + "';";

	double ans = 0; // result of total_answers query
	char* zErrMsg = 0;

	/* Execute SQL statement */
	int res = sqlite3_exec(m_database, query.c_str(), numCallback, &ans, &zErrMsg);

	if (res != SQLITE_OK) // sql error
	{
		std::cout << "Error: " << zErrMsg << std::endl;
		sqlite3_free(zErrMsg);
		return false;
	}

	return (int)ans;
}

/*
The function receives the username of a user and returns the number of games they played.
the function gets the answer from the DB.
input: the username, string.
output: the number of games the user played, int.
*/
int SqliteDatabase::getNumOfPlayerGames(std::string username) const
{
	std::string query = "SELECT GAMES  FROM STATISTICS WHERE NAME ='" + username + "';";

	double ans = 0; // result of games query
	char* zErrMsg = 0;

	/* Execute SQL statement */
	int res = sqlite3_exec(m_database, query.c_str(), numCallback, &ans, &zErrMsg);

	if (res != SQLITE_OK) // sql error
	{
		std::cout << "Error: " << zErrMsg << std::endl;
		sqlite3_free(zErrMsg);
		return false;
	}

	return (int)ans;
}


/*
Opens the database to use.
Input: none
Output: returns true if the database was opened successfully, else false.
*/
bool SqliteDatabase::open()
{
	int doesFileExist = _access(m_dbName.c_str(), 0); // check if sqlite db file exists
	
	int res = sqlite3_open(m_dbName.c_str(), &m_database);
	if (res)
	{
		// there's an error trying to open the DB, and it failed
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(this->m_database));
		return false;
	}
	
	if (doesFileExist == FILE_NOT_EXISTS) // initialize database (hasn't been initialized yet)
	{
		bool ans = initDatabase(); // initialize the database
		bool ans2 = questions(); // enter the question into the database
		if (!ans || !ans2) // failed to initialize database
			return false;
	}

	return true;
}

/*
The function checks if the user exists in the database.
Input: the username to check if exists.
Output: 
*/
bool SqliteDatabase::doesUserExist(std::string username) const
{
	std::string query = "SELECT count(NAME) FROM USERS WHERE NAME='" + username +"';";

	int ans = 0; // result of username query
	char* zErrMsg = 0;
	
	/* Execute SQL statement */
	int res = sqlite3_exec(m_database, query.c_str(), usersCallback, &ans, &zErrMsg);

	if (res != SQLITE_OK) // sqlite error
	{
		sqlite3_free(zErrMsg);
		return false;
	}
	else // valid sqlite query
	{
		if (ans)
			return true;
	}

	return false;
}

/*
Input: the username, it's password
Output: true if succeeded, else false
*/
bool SqliteDatabase::doesPasswordMatchToUser(std::string username, std::string password) const
{
	std::string query = "SELECT count(NAME) FROM USERS WHERE PASSWORD='" + password + "';";

	int ans = 0; // result of username query
	char* zErrMsg = 0;

	/* Execute SQL statement */
	int res = sqlite3_exec(m_database, query.c_str(), usersCallback, &ans, &zErrMsg);

	if (res != SQLITE_OK) // sql error
	{
		std::cout << "Error: " << zErrMsg << std::endl;
		sqlite3_free(zErrMsg);
		return false;
	}
	
	// valid sql query, password from the DB is the same as the one that was recived as a parameter.
	if (ans)
		return true;

	return false;
}

/*
The function inserts a new user into the database.
Input: the username, it's password & email.
Output: none
*/
void SqliteDatabase::addNewUser(std::string name, std::string password, std::string email) const
{
	char* zErrMsg = 0;
	std::string query = "INSERT INTO USERS (NAME, PASSWORD, EMAIL) VALUES "\
						"('" + name + "', '" + password + "', '" + email + "');";
	
	/* Execute SQL statement */
	std::cout << "[+] Inserting user '" << name << "' into DB . . ." << std::endl;
	int res = sqlite3_exec(m_database, query.c_str(), callback, 0 , &zErrMsg);

	if (res != SQLITE_OK) // sql error
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		throw std::exception("Something went wrong with inserting into 'Users' table.\n");
	}
}
