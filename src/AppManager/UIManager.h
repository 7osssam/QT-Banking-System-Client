#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QJsonDocument>
#include <QMessageBox>
#include "LoginWidget.h"
#include "AdminWidget.h"
#include "UserWidget.h"
#include "RequestFactory.h"
#include "ResponseManager.h"

/**
 * @file UIManager.h
 * @brief Manages the user interface of the application.
 * @details This class is responsible for creating and managing different UI widgets such as login, user, and admin widgets. It handles the navigation between these widgets and processes responses from the server.
 */

class UIManager : public QObject
{
	Q_OBJECT

private:
	QMainWindow*	 mainWindow;	  /**< Main application window. */
	QStackedWidget*	 stackedWidget;	  /**< Widget to hold and switch between different UI pages. */
	LoginWidget*	 loginWidget;	  /**< Widget for user login. */
	UserWidget*		 userWidget;	  /**< Widget for regular user functionalities. */
	AdminWidget*	 adminWidget;	  /**< Widget for admin functionalities. */
	QString			 host;			  /**< Server host address. */
	quint16			 port;			  /**< Server port number. */
	QString			 first_name;	  /**< First name of the user/admin. */
	QString			 role;			  /**< Role of the logged-in user. */
	QString			 message;		  /**< Message to be displayed. */
	ResponseManager* responseManager; /**< Handles server responses. */
	RequestFactory*	 requestFactory;  /**< Creates requests to be sent to the server. */

	/**
     * @brief Private constructor for the UIManager class.
     * @details Initializes UI components and connects signals and slots for handling user actions and server responses.
     * @param parent Optional parent QObject.
     */
	explicit UIManager(QObject* parent = nullptr);

public:
	/**
     * @brief Destructor for the UIManager class.
     * @details Cleans up the main window and its widgets.
     */
	~UIManager();

	/**
     * @brief Deleted copy constructor to prevent copying.
     */
	UIManager(const UIManager&) = delete;

	/**
     * @brief Deleted assignment operator to prevent copying.
     */
	void operator=(const UIManager&) = delete;

	/**
     * @brief Gets the singleton instance of UIManager.
     * @param parent Optional parent QObject.
     * @return Pointer to the UIManager instance.
     */
	static UIManager* getInstance(QObject* parent = nullptr);

signals:
	/**
     * @brief Signal emitted when a request is ready to be sent to the server.
     * @param Data JSON object containing the request data.
     */
	void makeRequest(QJsonObject Data);

	/**
     * @brief Signal emitted to request a connection to the server.
     * @param host Server host address.
     * @param port Server port number.
     */
	void requestConnection(const QString& host, quint16 port);

	/**
     * @brief Signal emitted to request a disconnection from the server.
     */
	void requestDisconnection();

public slots:
	/**
     * @brief Slot to handle responses from the server.
     * @param Data JSON object containing the response data.
     */
	void responseReady(QJsonObject Data);

	/**
     * @brief Slot to handle requests that are ready to be sent.
     * @param Data JSON object containing the request data.
     */
	void requestReady(QJsonObject Data);

	/**
     * @brief Logs out the current user.
     * @details Disconnects signals and resets the UI to the login widget.
     */
	void logout();

	/**
     * @brief Creates the login widget.
     * @details Initializes and displays the login widget.
     */
	void createLoginWidget();

	/**
     * @brief Creates the admin widget.
     * @param email Admin email address.
     * @param first_name Admin first name.
     */
	void createAdminWidget(QString email, QString first_name);

	/**
     * @brief Creates the user widget.
     * @param email User email address.
     * @param first_name User first name.
     * @param account_number User account number.
     * @param balance User account balance.
     */
	void createUserWidget(QString email, QString first_name, QString account_number, QString balance);

	/**
     * @brief Displays a successful notification.
     * @param message The success message to be displayed.
     */
	void onSuccessfullNotification(QString message);

	/**
     * @brief Displays a failed notification.
     * @param message The failure message to be displayed.
     */
	void onFailedNotification(QString message);

	/**
     * @brief Closes the admin widget.
     */
	void closeAdminWidget();

	/**
     * @brief Closes the user widget.
     */
	void closeUserWidget();

	/**
     * @brief Closes the login widget.
     */
	void closeLoginWidget();

	/**
     * @brief Connects to the server.
     * @param host Server host address.
     * @param port Server port number.
     */
	void connectToTheServer(const QString& host, quint16 port);

	/**
     * @brief Disconnects from the server.
     */
	void disconnectFromTheServer();

	/**
     * @brief Handles the server connection response.
     * @param status Connection status (true if connected, false otherwise).
     */
	void onConnectionResponse(bool status);
};

#endif // UIMANAGER_H
