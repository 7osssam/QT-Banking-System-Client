#include "loginwidget.h"
#include <QMessageBox>
#include <QRegularExpressionValidator>

#include "RequestManager.h"

#define TEXT_COLOR		   "#0A1E4A" // Dark blue
#define INK_COLOR		   "#1C3A75" // Medium blue
#define INPUT_LINE_COLOR   "#A3C9E9" // Light blue
#define LABEL_COLOR		   "#163A5F" // Deep blue

// light green
#define CONNECTED_COLOR	   "#00FF00"
// dark red
#define DISCONNECTED_COLOR "#FF0000"

LoginWidget::LoginWidget(QWidget* parent) :
	QWidget(parent), isConnected(false), requestManager(RequestManager::getInstance(this))
{
	// set object name
	setObjectName("LoginWidget");

	// Set the window size
	setFixedSize(800, 600);

	// Create the background label and movie
	backgroundLabel = new QLabel(this);
	backgroundLabel->setGeometry(0, 0, 800, 600);

	backgroundMovie = new QMovie(":/gifs/background.gif");
	// fill the background with the gif
	backgroundMovie->setScaledSize(QSize(800, 600));
	backgroundLabel->setMovie(backgroundMovie);
	backgroundMovie->start();

	// Create a widget to hold the login form
	QWidget* loginForm = new QWidget(this);
	loginForm->setGeometry(100, 100, 600, 400);

	QVBoxLayout* layout = new QVBoxLayout(loginForm);

	// Application Title
	titleLabel = new QLabel("Banking System", loginForm);
	titleLabel->setAlignment(Qt::AlignCenter);
	titleLabel->setPixmap(QPixmap(":/icons/bank.png", "PNG").scaled(300, 300, Qt::KeepAspectRatio));

	layout->addWidget(titleLabel);

	// Email Field
	emailField = new QtMaterialTextField(this);
	emailField->setLabelText("Email");
	emailField->setFont(QFont("Fira Sans", 18, QFont::ExtraBold));

	emailField->setTextColor(QColor(TEXT_COLOR));
	emailField->setInkColor(QColor(INK_COLOR));
	emailField->setInputLineColor(QColor(INPUT_LINE_COLOR));
	emailField->setLabelColor(QColor(LABEL_COLOR));
	layout->addWidget(emailField);

	// Password Field
	passwordField = new QtMaterialTextField(this);
	passwordField->setLabelText("Password");
	passwordField->setEchoMode(QLineEdit::Password);
	passwordField->setTextColor(QColor(TEXT_COLOR));
	passwordField->setInkColor(QColor(INK_COLOR));
	passwordField->setInputLineColor(QColor(INPUT_LINE_COLOR));
	passwordField->setLabelColor(QColor(LABEL_COLOR));
	layout->addWidget(passwordField);

	connect(emailField, &QtMaterialTextField::textChanged, this, &LoginWidget::onLoginTextChanged);
	connect(passwordField, &QtMaterialTextField::textChanged, this, &LoginWidget::onLoginTextChanged);

	// Login Button
	loginButton = new QtMaterialFlatButton("Login", Material::ButtonRaisedDefault, this);
	loginButton->setOverlayStyle(Material::GrayOverlay);
	loginButton->setDisabled(true);						// Initially disabled until connected to the server
	loginButton->setIcon(QtMaterialTheme::icon("action", "input"));
	loginButton->setCornerRadius(17);
	loginButton->setBackgroundColor(QColor("#0A1E4A")); // Dark blue
	loginButton->setOverlayColor(QColor("#1C3A75"));	// Medium blue
	loginButton->setFont(QFont("Fira Sans", 18, QFont::ExtraBold));

	connect(loginButton, &QtMaterialFlatButton::clicked, this, &LoginWidget::onLoginButton);
	layout->addWidget(loginButton);
	// add little space after login button
	layout->addSpacing(50);

	setLayout(layout);

	// Connection Icon Button
	connectionIconButton = new QtMaterialIconButton(QtMaterialTheme::icon("file", "cloud_off"), this);
	connectionIconButton->setGeometry(760, 20, 30, 30);
	connectionIconButton->setColor(QColor(DISCONNECTED_COLOR));
	connect(connectionIconButton, &QtMaterialIconButton::clicked, this, &LoginWidget::onConnectionIconButton);

	// Connection Dialog
	connectionDialog = new QDialog(this);
	connectionDialog->setWindowTitle("Connect to Server");
	connectionDialog->setFixedSize(200, 100);

	QVBoxLayout* dialogLayout = new QVBoxLayout(connectionDialog);

	// IP and Port Field
	ipPortField = new QLineEdit(connectionDialog);
	ipPortField->setPlaceholderText("Enter IP Address and Port (e.g., 192.168.1.1:8080)");

	QString						 ipRange = "(?:[0-9]{1,3}\\.){3}[0-9]{1,3}:[0-9]{4}";
	QRegularExpression			 ipRegex("^" + ipRange + "$");
	QRegularExpressionValidator* ipValidator = new QRegularExpressionValidator(ipRegex, this);

	ipPortField->setValidator(ipValidator);
	ipPortField->setInputMask("000.000.000.000:0000");
	ipPortField->setAlignment(Qt::AlignCenter);
	ipPortField->setCursorPosition(0);

	dialogLayout->addWidget(ipPortField);

	connectButton = new QPushButton("Connect", connectionDialog);
	connect(connectButton, &QPushButton::clicked, this, &LoginWidget::onConnectButton);
	dialogLayout->addWidget(connectButton);

	connectionDialog->setLayout(dialogLayout);

	// Login Snackbar
	notificationSnackbar = new QtMaterialSnackbar(this);
	notificationSnackbar->setAutoHideDuration(3000);   // Auto-hide after 3 seconds
	notificationSnackbar->setClickToDismissMode(true); // Allow click to dismiss

	// Set the font for snackbar FiraSans-Ultra font
	notificationSnackbar->setFont(QFont("Fira Sans", 16, QFont::ExtraBold));
}

void LoginWidget::onLoginButton()
{
	loginData.insert("email", emailField->text());
	loginData.insert("password", passwordField->text());

	if (emailField->text().isEmpty() || passwordField->text().isEmpty())
	{
		onFailedRequest("Email or password cannot be empty");
		return;
	}

	requestManager->createRequest(RequestManager::Login, loginData);
}

void LoginWidget::onLoginTextChanged()
{
	emailField->setLabelVisible(emailField->text().isEmpty());
	emailField->setInputLineColor(emailField->text().isEmpty() ? QColor("#FF0000") : QColor("#A3C9E9"));
	passwordField->setLabelVisible(passwordField->text().isEmpty());
}

void LoginWidget::onConnectionIconButton()
{
	if (isConnected)
	{
		emit disconnectFromServer();
	}
	else
	{
		connectionDialog->show();
	}
}

void LoginWidget::onConnectButton()
{
	QString		ipPort = ipPortField->text();
	QStringList ipPortList = ipPort.split(":");

	if (ipPortList.size() != 2 || ipPortList[0].isEmpty() || ipPortList[1].isEmpty())
	{
		QMessageBox::warning(this, "Connect Failed", "Invalid IP address or port format", QMessageBox::Ok);

		return;
	}

	QString ip = ipPortList[0];
	quint16 port = ipPortList[1].toUShort();

	emit connectToServer(ip, port);
}

void LoginWidget::onConnected()
{
	isConnected = true;
	connectionIconButton->setColor(QColor(CONNECTED_COLOR));
	connectionIconButton->setIcon(QtMaterialTheme::icon("file", "cloud"));
	connectionDialog->hide();

	loginButton->setDisabled(false);

	onSuccessfullRequest("Connected");
}

void LoginWidget::onDisconnected()
{
	isConnected = false;
	connectionIconButton->setColor(QColor(DISCONNECTED_COLOR));
	connectionIconButton->setIcon(QtMaterialTheme::icon("file", "cloud_off"));

	loginButton->setDisabled(true);

	onFailedRequest("Disconnected");
}

void LoginWidget::onFailedRequest(QString message)
{
	// set color to red with small opacity
	notificationSnackbar->setBackgroundColor(QColor(255, 0, 0, 100));

	notificationSnackbar->addMessage(message);
}

void LoginWidget::onSuccessfullRequest(QString message)
{
	// set color to green with small opacity
	notificationSnackbar->setBackgroundColor(QColor(0, 255, 0, 100));

	notificationSnackbar->addMessage(message);

	//if the message string contains "Login Successfull" then emit the signal to the UIManager
	if (message.contains("Login Successfull"))
	{
		requestManager->createRequest(RequestManager::UserInit, loginData);
	}
}

void LoginWidget::clearFields()
{
	emailField->clear();
	passwordField->clear();
}
