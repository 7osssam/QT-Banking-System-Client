#include "loginwidget.h"
#include <QMessageBox>
#include <QRegularExpressionValidator>

#define TEXT_COLOR		   "#030637"
#define INK_COLOR		   "#910A67"
#define INPUT_LINE_COLOR   "#E0E0E0"
#define LABEL_COLOR		   "#3C0753"
#define CONNECTED_COLOR	   "#00FF00"
#define DISCONNECTED_COLOR "#808080"

LoginWidget::LoginWidget(QWidget* parent) : QWidget(parent), isConnected(false)
{
	// Set the window size
	setFixedSize(800, 600);

	// Create the background label and movie
	backgroundLabel = new QLabel(this);
	backgroundLabel->setGeometry(0, 0, 800, 600);

	backgroundMovie = new QMovie("space.gif");
	backgroundLabel->setMovie(backgroundMovie);
	backgroundMovie->start();

	// Create a widget to hold the login form
	QWidget* loginForm = new QWidget(this);
	loginForm->setGeometry(100, 100, 600, 400);

	QVBoxLayout* layout = new QVBoxLayout(loginForm);

	// Application Title
	titleLabel = new QLabel("Banking System", loginForm);
	titleLabel->setAlignment(Qt::AlignCenter);
	titleLabel->setStyleSheet("font-size: 24px; color: #FFFFFF;");
	titleLabel->setCursor(Qt::ArrowCursor);
	titleLabel->setPixmap(QPixmap("logo.png"));

	layout->addWidget(titleLabel);

	// Email Field
	emailField = new QtMaterialTextField(this);
	emailField->setLabelText("Email");

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

	connect(emailField, &QtMaterialTextField::textChanged, this, &LoginWidget::onTextChanged);
	connect(passwordField, &QtMaterialTextField::textChanged, this, &LoginWidget::onTextChanged);

	// Login Button
	loginButton = new QtMaterialFlatButton("Login", Material::ButtonTextDefault, this);
	loginButton->setUseThemeColors(true);
	loginButton->setRole(Material::Primary);
	loginButton->setOverlayStyle(Material::TintedOverlay);
	loginButton->setDisabled(true); // Initially disabled until connected to the server
	loginButton->setIcon(QtMaterialTheme::icon("action", "input"));
	loginButton->setCornerRadius(20);

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

	QVBoxLayout* dialogLayout = new QVBoxLayout(connectionDialog);

	// IP and Port Field
	ipPortField = new QLineEdit(connectionDialog);
	ipPortField->setPlaceholderText("Enter IP Address and Port (e.g., 192.168.1.1:8080)");

	QString						 ipRange = "(?:[0-9]{1,3}\\.){3}[0-9]{1,3}:[0-9]{4}";
	QRegularExpression			 ipRegex("^" + ipRange + "$");
	QRegularExpressionValidator* ipValidator = new QRegularExpressionValidator(ipRegex, this);

	ipPortField->setValidator(ipValidator);
	ipPortField->setInputMask("000.000.000.000:0000");
	ipPortField->setFixedWidth(300);
	ipPortField->setFixedHeight(30);
	ipPortField->setCursorPosition(0);

	dialogLayout->addWidget(ipPortField);

	connectButton = new QPushButton("Connect", connectionDialog);
	connect(connectButton, &QPushButton::clicked, this, &LoginWidget::onConnectButton);
	dialogLayout->addWidget(connectButton);

	connectionDialog->setLayout(dialogLayout);

	// Login Snackbar
	loginSnackbar = new QtMaterialSnackbar(this);
	loginSnackbar->setAutoHideDuration(3000);	// Auto-hide after 3 seconds
	loginSnackbar->setClickToDismissMode(true); // Allow click to dismiss
												//loginSnackbar->set
}

void LoginWidget::onLoginButton()
{
	QString email = emailField->text();
	QString password = passwordField->text();

	if (email.isEmpty() || password.isEmpty())
	{
		QMessageBox::warning(this, "Login Failed", "Email or password cannot be empty", QMessageBox::Ok);
		return;
	}

	QJsonObject loginData;
	loginData.insert("Request", 1);
	QJsonObject data;
	data.insert("email", email);
	data.insert("password", password);

	loginData.insert("Data", data);

	emit sendLoginRequest(loginData);
}

void LoginWidget::onTextChanged()
{
	emailField->setLabelVisible(emailField->text().isEmpty());
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

	if (ipPortList.size() != 2)
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

	QMessageBox::warning(this, "Connected", "Connected to server", QMessageBox::Ok);
}

void LoginWidget::onDisconnected()
{
	isConnected = false;
	connectionIconButton->setColor(QColor(DISCONNECTED_COLOR));
	connectionIconButton->setIcon(QtMaterialTheme::icon("file", "cloud_off"));

	loginButton->setDisabled(true);

	QMessageBox::warning(this, "Connected", "Disconnected from server", QMessageBox::Ok);
}
void LoginWidget::onFailedLogin(QString message)
{
	loginSnackbar->addMessage("Login Failed: " + message);
}

//onLoginButtonDialog
void LoginWidget::onLoginButtonDialog()
{
	loginDialog->hide();
	//emit disconnectFromServer();
}