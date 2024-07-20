/**
 * @file RequestManager.h
 * @brief Header file for the RequestManager class.
 *
 * @details Declares the RequestManager class, which manages the creation and dispatching of requests in the system.
 */

#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <QJsonObject>
#include <QString>
#include <QMap>
#include <QVariantMap>
#include <QVariant>
#include <QObject>

/**
 * @class RequestManager
 * @brief Manages the creation and dispatching of requests in the system.
 *
 * The RequestManager class follows the Singleton design pattern to ensure that only one instance
 * of the class exists throughout the application. It is responsible for creating various types
 * of requests and emitting a signal to notify when a request is made.
 *
 * The requests are defined by the AvailableRequests enum and include operations such as user login,
 * account retrieval, balance checking, transaction history, and more. The requests are created
 * in the form of QJsonObject and emitted via the makeRequest signal.
 */
class RequestManager : public QObject
{
	Q_OBJECT

private:
	/**
	 * @brief Private constructor to enforce Singleton pattern.
	 *
	 * @param parent The parent QObject, default is nullptr.
	 */
	explicit RequestManager(QObject* parent = nullptr);

signals:
	/**
	 * @brief Signal emitted when a request is made.
	 *
	 * @param Data The QJsonObject representing the request data.
	 */
	void makeRequest(QJsonObject Data);

public:
	// Delete the copy constructor and assignment operator to prevent copying
	RequestManager(const RequestManager&) = delete;
	void operator=(const RequestManager&) = delete;

	/**
	 * @brief Get the singleton instance of RequestManager.
	 *
	 * @param parent The parent QObject, default is nullptr.
	 * @return RequestManager* Pointer to the singleton instance.
	 */
	static RequestManager* getInstance(QObject* parent = nullptr);

	/**
	 * @enum AvailableRequests
	 * @brief Defines the types of requests that can be created.
	 */
	enum AvailableRequests
	{
		Login = 1,				///< User login request
		GetAccountnumber,		///< Request to get account number
		GetBalance,				///< Request to get account balance
		GetTransactionsHistory, ///< Request to get transaction history
		MakeTransaction,		///< Request to make a transaction
		TransferAmount,			///< Request to transfer an amount (not used for now)
		GetDatabase,			///< Request to get the database
		CreateNewUser,			///< Request to create a new user
		DeleteUser,				///< Request to delete a user
		UpdateUser,				///< Request to update user information
		// Special requests
		UserInit,			///< Request to initialize user
		UpdateEmail,		///< Request to update user email
		UpdatePassword,		///< Request to update user password
		JsonParseError = -1 ///< Indicates a JSON parse error
	};

	/**
	 * @brief Creates a request based on the provided type and data.
	 *
	 * @param requestType The type of request to create, defined by AvailableRequests enum.
	 * @param data The data to be included in the request, in the form of QVariantMap.
	 */
	void createRequest(AvailableRequests requestType, QVariantMap data);
};

#endif // REQUESTMANAGER_H
