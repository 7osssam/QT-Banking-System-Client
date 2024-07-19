#include "AppManager.h"

AppManager::AppManager(QObject* parent) :
	QObject(parent), uiManager(UIManager::getInstance(this)), clientHandler(new ClientHandler()),
	AppManagerThread(new QThread())
{
	clientHandler->moveToThread(AppManagerThread);

	connect(uiManager, &UIManager::makeRequest, clientHandler, &ClientHandler::sendRequest);
	connect(uiManager, &UIManager::requestConnection, clientHandler, &ClientHandler::requestClientConnection);
	connect(uiManager, &UIManager::requestDisconnection, clientHandler, &ClientHandler::requestClientDisconnection);

	connect(clientHandler, &ClientHandler::sendResponseBack, uiManager, &UIManager::responseReady);

	connect(AppManagerThread, &QThread::started, clientHandler, &ClientHandler::run);
	connect(AppManagerThread, &QThread::finished, AppManagerThread, &QThread::deleteLater);
}

void AppManager::start()
{
	AppManagerThread->start();
}

AppManager& AppManager::getInstance(QObject* parent)
{
	static AppManager instance(parent);
	return instance;
}

AppManager::~AppManager()
{
	delete uiManager;
	delete clientHandler;
}
