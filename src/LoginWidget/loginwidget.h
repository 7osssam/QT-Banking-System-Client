#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "qtmaterialtextfield.h"
#include "qtmaterialflatbutton.h"
#include "qtmaterialiconbutton.h"
#include "qtmaterialdialog.h"
#include "qtmaterialsnackbar.h"

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
	QtMaterialDialog*	  loginDialog;
	QtMaterialSnackbar*	  loginSnackbar;

	bool				  isConnected;

	// Connection dialog components
	QDialog*	 connectionDialog;
	QLineEdit*	 ipPortField;
	QPushButton* connectButton;

signals:
	void connectToServer(const QString& host, quint16 port);
	void disconnectFromServer();
	void sendLoginRequest(QJsonObject loginData);

public slots:
	void onConnected();
	void onDisconnected();
	void onFailedLogin(QString message);

private slots:
	void onLoginButton();
	void onLoginButtonDialog();
	void onTextChanged();
	void onConnectionIconButton();
	void onConnectButton();
};

#endif // LOGINWIDGET_H
