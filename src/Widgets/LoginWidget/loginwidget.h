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
#include "qtmaterialwidget.h"

#include "RequestManager.h"

#include <QMovie>
#include <QLineEdit>
#include <QJsonObject>
#include <QDialog>

/**
 * @brief The LoginWidget class provides a user interface for logging into the banking system.
 *
 * The LoginWidget is responsible for user authentication, including connecting to the server,
 * handling user input, and displaying appropriate feedback. It provides fields for email and
 * password entry, a login button, and an indicator for connection status.
 */
class LoginWidget : public QWidget
{
	Q_OBJECT

public:
	/**
     * @brief Constructs a LoginWidget instance.
     * @param parent The parent QWidget, defaults to nullptr.
     */
	explicit LoginWidget(QWidget* parent = nullptr);

signals:
	/**
     * @brief Signal emitted to connect to the server.
     * @param host The server IP address.
     * @param port The server port.
     */
	void connectToServer(const QString& host, quint16 port);

	/**
     * @brief Signal emitted to disconnect from the server.
     */
	void disconnectFromServer();

	/**
     * @brief Signal emitted to send login request.
     * @param loginData The JSON object containing login data.
     */
	void sendLoginRequest(QJsonObject loginData);

public slots:
	/**
     * @brief Slot to handle successful connection to the server.
     */
	void onConnected();

	/**
     * @brief Slot to handle disconnection from the server.
     */
	void onDisconnected();

	/**
     * @brief Slot to handle failed login request.
     * @param message The error message to display.
     */
	void onFailedRequest(QString message);

	/**
     * @brief Slot to handle successful login request.
     * @param message The success message to display.
     */
	void onSuccessfullRequest(QString message);

	/**
     * @brief Clears the input fields for email and password.
     */
	void clearFields();

private slots:
	/**
     * @brief Slot to handle login button click.
     */
	void onLoginButton();

	/**
     * @brief Slot to handle text changes in the login fields.
     */
	void onLoginTextChanged();

	/**
     * @brief Slot to handle connection icon button click.
     */
	void onConnectionIconButton();

	/**
     * @brief Slot to handle connect button click in the connection dialog.
     */
	void onConnectButton();

private:
	QtMaterialTextField*  emailField;			/**< Field for entering email address. */
	QtMaterialTextField*  passwordField;		/**< Field for entering password. */
	QLabel*				  titleLabel;			/**< Label displaying the application title. */
	QtMaterialFlatButton* loginButton;			/**< Button to submit login credentials. */
	QLabel*				  backgroundLabel;		/**< Label for background image or animation. */
	QMovie*				  backgroundMovie;		/**< Movie (GIF) for background animation. */
	QtMaterialIconButton* connectionIconButton; /**< Button to manage connection status. */
	QtMaterialSnackbar*	  notificationSnackbar; /**< Snackbar for displaying messages to the user. */
	RequestManager*		  requestManager;		/**< Singleton instance managing requests. */
	bool				  isConnected;			/**< Connection status flag. */

	// Connection dialog components
	QDialog*	 connectionDialog; /**< Dialog for entering connection details. */
	QLineEdit*	 ipPortField;	   /**< Field for entering IP address and port. */
	QPushButton* connectButton;	   /**< Button to initiate connection. */

	QString		email;			   /**< Stored email address for login. */
	QString		password;		   /**< Stored password for login. */
	QVariantMap loginData;		   /**< Data to be sent for login request. */
};

#endif // LOGINWIDGET_H
