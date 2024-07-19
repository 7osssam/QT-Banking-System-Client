#include "UIManager.h"

UIManager::UIManager(QObject* parent) :
	QObject(parent), mainWindow(nullptr), loginWidget(nullptr), userWidget(nullptr), adminWidget{nullptr},
	responseManager(nullptr)
{
	responseManager = new ResponseManager(this);

	requestFactory = RequestFactory::getInstance(this);

	connect(requestFactory, &RequestFactory::makeRequest, this, &UIManager::requestReady);

	connect(responseManager, &ResponseManager::userloginSuccess, this, &UIManager::createUserWidget);
	connect(responseManager, &ResponseManager::adminLoginSuccess, this, &UIManager::createAdminWidget);
	connect(responseManager, &ResponseManager::SuccessfullRequest, this, &UIManager::onSuccessfullNotification);
	connect(responseManager, &ResponseManager::FailedRequest, this, &UIManager::onFailedNotification);
	connect(responseManager, &ResponseManager::ConnectionResponse, this, &UIManager::onConnectionResponse);

	mainWindow = new QMainWindow;
	stackedWidget = new QStackedWidget(mainWindow);
	createLoginWidget();

	mainWindow->setCentralWidget(stackedWidget);
	mainWindow->setFixedSize(800, 600);
	mainWindow->show();
}

UIManager::~UIManager()
{
	delete mainWindow;
	// The destructor will automatically delete stackedWidget and its children.
}

UIManager* UIManager::getInstance(QObject* parent)
{
	static UIManager* instance = new UIManager(parent);
	return instance;
}

void UIManager::responseReady(QJsonObject Data)
{
	responseManager->handleResponse(Data);
}

void UIManager::requestReady(QJsonObject Data)
{
	emit makeRequest(Data);
}

void UIManager::logout()
{
	qDebug() << "Logging out :" << sender()->objectName();
	// Disconnect signals to avoid any pending connections.
	if (userWidget != nullptr)
	{
		disconnect(adminWidget, &AdminWidget::logout, this, &UIManager::logout);
		disconnect(responseManager, &ResponseManager::TransactionsFetched, userWidget,
				   &UserWidget::onTransactionsFetched);
		disconnect(responseManager, &ResponseManager::BalanceFetched, userWidget, &UserWidget::onBalanceFetched);
		closeUserWidget();
	}
	if (adminWidget != nullptr)
	{
		disconnect(adminWidget, &AdminWidget::logout, this, &UIManager::logout);
		disconnect(responseManager, &ResponseManager::TransactionsFetched, adminWidget,
				   &AdminWidget::onTransactionsFetched);
		disconnect(responseManager, &ResponseManager::DatabaseFetched, adminWidget,
				   &AdminWidget::onDatabaseContentUpdated);
		closeAdminWidget();
	}
	stackedWidget->setCurrentWidget(loginWidget);
}
void UIManager::createLoginWidget()
{
	if (loginWidget == nullptr)
	{
		//loginWidget = LoginWidget::getInstance(mainWindow);
		loginWidget = new LoginWidget(mainWindow);
		connect(loginWidget, &LoginWidget::connectToServer, this, &UIManager::connectToTheServer);
		connect(loginWidget, &LoginWidget::disconnectFromServer, this, &UIManager::disconnectFromTheServer);
		connect(loginWidget, &LoginWidget::sendLoginRequest, this, &UIManager::requestReady);
	}

	mainWindow->setWindowTitle("Login Page");
	stackedWidget->addWidget(loginWidget);
	stackedWidget->setCurrentWidget(loginWidget);
}

void UIManager::createAdminWidget(QString email, QString first_name)
{
	if (adminWidget == nullptr)
	{
		adminWidget = new AdminWidget(email, first_name, mainWindow);
		connect(adminWidget, &AdminWidget::logout, this, &UIManager::logout);
		connect(responseManager, &ResponseManager::TransactionsFetched, adminWidget,
				&AdminWidget::onTransactionsFetched);

		connect(responseManager, &ResponseManager::DatabaseFetched, adminWidget,
				&AdminWidget::onDatabaseContentUpdated);
	}

	mainWindow->setWindowTitle("Admin Page");
	stackedWidget->addWidget(adminWidget);
	stackedWidget->setCurrentWidget(adminWidget);
}

void UIManager::createUserWidget(QString email, QString first_name, QString account_number, QString balance)
{
	if (userWidget == nullptr)
	{
		userWidget = new UserWidget(email, first_name, account_number, balance, mainWindow);
		connect(userWidget, &UserWidget::logout, this, &UIManager::logout);
		connect(responseManager, &ResponseManager::TransactionsFetched, userWidget, &UserWidget::onTransactionsFetched);
		connect(responseManager, &ResponseManager::BalanceFetched, userWidget, &UserWidget::onBalanceFetched);
	}

	mainWindow->setWindowTitle("User Page");
	stackedWidget->addWidget(userWidget);
	stackedWidget->setCurrentWidget(userWidget);
}

void UIManager::onSuccessfullNotification(QString message)
{
	if (userWidget != nullptr)
	{
		userWidget->onSuccessfullRequest(message);
	}
	if (loginWidget != nullptr)
	{
		loginWidget->onSuccessfullRequest(message);
	}
	if (adminWidget != nullptr)
	{
		adminWidget->onSuccessfullRequest(message);
	}
}

void UIManager::onFailedNotification(QString message)
{
	if (userWidget != nullptr)
	{
		userWidget->onFailedRequest(message);
	}
	if (loginWidget != nullptr)
	{
		loginWidget->onFailedRequest(message);
	}
	if (adminWidget != nullptr)
	{
		adminWidget->onFailedRequest(message);
	}
}

void UIManager::closeAdminWidget()
{
	stackedWidget->removeWidget(adminWidget);
	adminWidget->deleteLater();
	adminWidget = nullptr;
}

void UIManager::closeUserWidget()
{
	stackedWidget->removeWidget(userWidget);
	userWidget->deleteLater();
	userWidget = nullptr;
}

void UIManager::closeLoginWidget()
{
	loginWidget->deleteLater();
	loginWidget = nullptr;
}

void UIManager::connectToTheServer(const QString& host, quint16 port)
{
	emit requestConnection(host, port);
}

void UIManager::disconnectFromTheServer()
{
	emit requestDisconnection();
}

void UIManager::onConnectionResponse(bool status)
{
	if (status)
	{
		if (loginWidget != nullptr)
		{
			loginWidget->onConnected();
		}
	}
	else
	{
		if (loginWidget != nullptr)
		{
			loginWidget->onDisconnected();
		}
	}
}