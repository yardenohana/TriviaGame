#include "RequestHandlerFactory.h"

unsigned int RoomManager::latestID = 1; // initialize the static roon ID variable (used for RoomManager)

// default constructor
RequestHandlerFactory::RequestHandlerFactory()
{
	this->m_loginManager = new LoginManager();
	this->m_database = new SqliteDatabase();
	this->m_roomManager = new RoomManager();
	this->m_StatisticsManager = new StatisticsManager();
	this->m_gameManager = new GameManager(this->m_database);
}

RequestHandlerFactory::RequestHandlerFactory(RoomManager* roomManager)
{
	this->m_loginManager = new LoginManager();
	this->m_database = new SqliteDatabase();
	this->m_roomManager = roomManager;
	this->m_StatisticsManager = new StatisticsManager();
	this->m_gameManager = new GameManager(this->m_database);
}

RequestHandlerFactory::~RequestHandlerFactory()
{
	delete this->m_database;
	delete this->m_roomManager;
	delete this->m_StatisticsManager;
	delete this->m_loginManager;
}

/*
function creates new LoginRequestHandle.
input: RequestHandlerFactory*, since it is the field of the new handler.
output: LoginRequestHandle*, with all the information that was given in the handler (the parameter)
*/
LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler(RequestHandlerFactory* handler)
{
	LoginRequestHandler* loginReq = new LoginRequestHandler(handler);
	return loginReq;
}

/*
function creates new MenuRequestHandler, using the data inside the object, and the user name of the new user.
input: username, string.
output: MenuRequestHandler*, with all the previous information from 'this', and the username.
*/
MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(std::string username)
{
	LoggedUser user(username);
	user.setAdmin(false);
	MenuRequestHandler* menuHandle = new MenuRequestHandler(*this, user, m_roomManager, m_StatisticsManager);
	return menuHandle;
}

/*
the functions creates a new GameRequestHandler*, from the parameters.
input: Room to be the room that helds the game, RequestHandlerFactory* that includes all the managers of the server,
LoggedUser that plays the game, and the Game object that holds all the information about the user scores.
output: new GameRequestHandler*, with all of the information about the game.
*/
GameRequestHandler* RequestHandlerFactory::createGameRequestHandler(Room room, RequestHandlerFactory* handler, LoggedUser user, Game game)
{
	GameRequestHandler* gameHandler = new GameRequestHandler(handler, game, user, this->m_gameManager);
	return gameHandler;
}

/*
the function, that actually suppose to be part of the RoomAdmin/RoomMember RequestHandler is located here to prevent code duplication.
input: the function recives the RequestInfo from the user, the Handler's room and user,
in case that the user wants to recive the state of the room he is in.
output: part RequestResult to return to the user. the missing part will be completed at the suitable handler, according to the user's state.
*/
RequestResult RequestHandlerFactory::getRoomStateRequest(RequestInfo infoReq, Room* room, LoggedUser user)
{
	JsonRequestPacketDeserializer deSer;
	JsonResponsePacketSerializer ser;
	RequestResult reqReasult;
	GetRoomStateResponse response;

	//assemble the GetRoomStateResponse object:
	RoomData roomdata = room->getMetaData();
	response.answerTimeout = roomdata.timePerQuestion;
	response.hasGameBegun = roomdata.isActive;
	response.players = room->getAllUsers();
	response.questionCount = roomdata.numOfQuestionsInGame;
	response.status = SUCCESS;

	//assemble the requestResult object:
	reqReasult.didActionWorked = true;
	reqReasult.buffer = ser.serializeResponse(response);

	//at this point, the missing part of the requestResult is the nextHandler, which will be inserted in the correct function, according to the user's state.
	return reqReasult;
}

/*
the function, that actually suppose to be part of the RoomAdmin/RoomMember RequestHandler is located here to prevent code duplication.
input: the function recives the RequestInfo from the user, the Handler's room and user,
in case that the user wants to start the game.
output: part RequestResult to return to the user. 
*/
RequestResult RequestHandlerFactory::startGameRequest(RequestInfo infoReq, Room* room, LoggedUser user)
{
	JsonRequestPacketDeserializer deSer;
	JsonResponsePacketSerializer ser;
	RequestResult reqReasult;
	StartGameResponse response;

	if (infoReq.id != START_GAME_REQUEST)
	{
		reqReasult.didActionWorked = false;
		return reqReasult;
	}

	//creating game, and setting the rest of the reqReasult variables:
	Game game = this->m_gameManager->createGame(room);
	response.status = SUCCESS;
	reqReasult.buffer = ser.serializeResponse(response);

	//the next handler of both RoomAdmin and RoomMember will be the GameHandlerRequest
	GameRequestHandler* gameHandler = this->createGameRequestHandler(*room, this, user, game);
	reqReasult.newHandler = gameHandler;
	
	reqReasult.didActionWorked = true;

	return reqReasult;
}

//getters / setters:
GameManager* RequestHandlerFactory::getGameManager() const
{
	return this->m_gameManager;
}

IDatabase* RequestHandlerFactory::getDatabase() const
{
	return this->m_database;
}

LoginManager* RequestHandlerFactory::getLoginManager() const
{
	return this->m_loginManager;
}

StatisticsManager* RequestHandlerFactory::getStatisticsManger() const
{
	return m_StatisticsManager;
}

RoomManager* RequestHandlerFactory::getRoomManager() const
{
	return this->m_roomManager;
}

void RequestHandlerFactory::setRoomManager(RoomManager* roomManager)
{
	this->m_roomManager = roomManager;
}

