#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "qtmaterialtextfield.h"
#include "qtmaterialflatbutton.h"
#include "qtmaterialiconbutton.h"
#include "qtmaterialsnackbar.h"

#include "RequestFactory.h"

#include <QMovie>
#include <QLineEdit>
#include <QJsonObject>
#include <QDialog>

class LoginWidget : public QWidget
{
	Q_OBJECT

public:
	explicit LoginWidget(QWidget* parent = nullptr);

private:
	QtMaterialTextField*  emailField;
	QtMaterialTextField*  passwordField;
	QLabel*				  titleLabel;
	QtMaterialFlatButton* loginButton;
	QLabel*				  backgroundLabel;
	QMovie*				  backgroundMovie;
	QtMaterialIconButton* connectionIconButton;
	QtMaterialSnackbar*	  loginSnackbar;
	RequestFactory*		  requestFactory;

	bool isConnected;

	// Connection dialog components
	QDialog*	 connectionDialog;
	QLineEdit*	 ipPortField;
	QPushButton* connectButton;

	QString		email;
	QString		password;
	QVariantMap loginData;

signals:
	void connectToServer(const QString& host, quint16 port);
	void disconnectFromServer();
	void sendLoginRequest(QJsonObject loginData);

public slots:
	void onConnected();
	void onDisconnected();
	void onFailedRequest(QString message);
	void onSuccessfullRequest(QString message);

private slots:
	void onLoginButton();
	void onLoginTextChanged();
	void onConnectionIconButton();
	void onConnectButton();
};

#endif // LOGINWIDGET_H
