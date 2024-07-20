#ifndef RESPONSEMANAGER_H
#define RESPONSEMANAGER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/**
 * @class ResponseManager
 * @brief Manages the responses received from the server.
 *
 * The ResponseManager class processes server responses and emits signals based on the response type.
 * It handles various response types defined in the AvailableRequests enum and provides feedback
 * through signals to update the application state accordingly.
 */
class ResponseManager : public QObject
{
	Q_OBJECT

	//private:
public: // for testing
	/**
	 * @enum AvailableRequests
	 * @brief Defines the types of requests that can be handled.
	 */
	enum AvailableRequests
	{
		Login = 1,				///< User login response
		GetAccountnumber,		///< Response to get account number
		GetBalance,				///< Response to get account balance
		GetTransactionsHistory, ///< Response to get transaction history
		MakeTransaction,		///< Response to make a transaction
		TransferAmount,			///< Response to transfer an amount (not used for now)
		GetDatabase,			///< Response to get the database
		CreateNewUser,			///< Response to create a new user
		DeleteUser,				///< Response to delete a user
		UpdateUser,				///< Response to update user information
		// Special requests
		UserInit,			 ///< Response to initialize user
		UpdateEmail,		 ///< Response to update user email
		UpdatePassword,		 ///< Response to update user password
		JsonParseError = -1, ///< Indicates a JSON parse error
		Connection = -2		 ///< Indicates a connection response
	};

	/**
	 * @brief Checks the status value in the response.
	 *
	 * @param Data The QJsonObject representing the response data.
	 * @return true if the status value is 1, false otherwise.
	 */
	bool getResponseStatus(QJsonObject Data);

	/**
	 * @brief Retrieves the message from the response.
	 *
	 * @param Data The QJsonObject representing the response data.
	 * @return The message as a QString.
	 */
	QString getResponseMessage(QJsonObject Data);

signals:

	/**
	 * @brief Signal emitted when a user login is successful.
	 *
	 * @param email User's email.
	 * @param first_name User's first name.
	 * @param account_number User's account number.
	 * @param balance User's balance.
	 */
	void userloginSuccess(QString email, QString first_name, QString account_number, QString balance);

	/**
	 * @brief Signal emitted when an admin login is successful.
	 *
	 * @param email Admin's email.
	 * @param first_name Admin's first name.
	 */
	void adminLoginSuccess(QString email, QString first_name);

	/**
	 * @brief Signal emitted for connection response.
	 *
	 * @param status Connection status (true for success, false for failure).
	 */
	void ConnectionResponse(bool status);

	/**
	 * @brief Signal emitted when transactions are fetched.
	 *
	 * @param transactions List of transactions.
	 */
	void TransactionsFetched(const QList<QMap<QString, QString>>& transactions);

	/**
	 * @brief Signal emitted when the database content is fetched.
	 *
	 * @param databaseContent List of database content.
	 */
	void DatabaseFetched(const QList<QMap<QString, QString>>& databaseContent);

	/**
	 * @brief Signal emitted when the balance is fetched.
	 *
	 * @param balance User's balance.
	 */
	void BalanceFetched(QString balance);

	/**
	 * @brief Signal emitted for a successful request.
	 *
	 * @param message Success message.
	 */
	void SuccessfullRequest(QString message);

	/**
	 * @brief Signal emitted for a failed request.
	 *
	 * @param message Failure message.
	 */
	void FailedRequest(QString message);

public:
	/**
	 * @brief Constructor for ResponseManager.
	 *
	 * @param parent The parent QObject, default is nullptr.
	 */
	explicit ResponseManager(QObject* parent = nullptr);

	/**
	 * @brief Handles the response received from the server.
	 *
	 * @param Data The QJsonObject representing the response data.
	 * @return void
	 */
	void handleResponse(QJsonObject Data);
};

#endif // RESPONSEMANAGER_H
