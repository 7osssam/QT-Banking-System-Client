#ifndef RESPONSEMANAGER_H
#define RESPONSEMANAGER_H

// this class is responsible for managing the responses received from the server
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class ResponseManager : public QObject
{
	Q_OBJECT

private:
	enum AvailableRequests
	{
		Login = 1,
		GetAccountnumber,
		GetBalance,
		GetTransactionsHistory,
		MakeTransaction,
		TransferAmount, // not used for now
		GetDatabase,
		CreateNewUser,
		DeleteUser,
		UpdateUser,
		// Special requests
		UserInit,
		UpdateEmail,
		UpdatePassword,
		JsonParseError = -1
	};

	// this function will check the status value in the response
	bool getResponseStatus(QJsonObject Data)
	{
		if (Data.contains("status"))
		{
			if (Data.value("status").toInt() == 1)
			{
				return true;
			}
		}
		return false;
	}

	QString getResponseMessage(QJsonObject Data)
	{
		if (Data.contains("message"))
		{
			return Data.value("message").toString();
		}
		return "";
	}

signals:
	void userloginSuccess(QString email, QString first_name, QString account_number, QString balance);
	void adminLoginSuccess(QString email, QString first_name);

	void loginFailure(QString message);
	void loginSuccess(QString role);

	void TransactionsFetched(const QList<QMap<QString, QString>>& transactions);
	void DatabaseFetched(const QList<QMap<QString, QString>>& databaseContent);
	void BalanceFetched(QString balance);

	//void SuccessfullRequestUserWidget(QString message);
	//void FailedRequestUserWidget(QString message);
	void SuccessfullRequest(QString message);
	void FailedRequest(QString message);

public:
	explicit ResponseManager(QObject* parent = nullptr) : QObject(parent)
	{
	}

	bool handleResponse(QJsonObject Data)
	{
		bool		status = false;
		int			responseCode = 0; // make sure that there is no response code = 0
		QJsonObject dataObject;
		int			whatToDo;

		qDebug().noquote() << "---> Response received from server:\n"
						   << QJsonDocument(Data).toJson(QJsonDocument::Indented);

		// get the response code
		if (Data.contains("Response"))
		{
			responseCode = Data.value("Response").toInt();
			status = true;
		}
		else
		{
			responseCode = JsonParseError;
			status = false;
		}

		// check for the response data object
		if (Data.contains("Data"))
		{
			dataObject = Data.value("Data").toObject();
		}
		else
		{
			responseCode = JsonParseError;
			status = false;
		}

		switch (responseCode)
		{
			case Login:
				if (getResponseStatus(dataObject))
				{
					QString first_name = dataObject.value("first_name").toString();
					QString role = dataObject.value("role").toString();

					emit loginSuccess(role);
				}
				else
				{
					emit loginFailure(getResponseMessage(dataObject));
				}
				break;

			case GetAccountnumber:
				// handle the get account number response
				break;

			case GetBalance:
				if (getResponseStatus(dataObject))
				{
					QString balance = QString::number(dataObject.value("balance").toDouble(), 'f', 2);
					emit	BalanceFetched(balance);
				}
				else
				{
				}

				break;

			case GetTransactionsHistory:
				if (getResponseStatus(dataObject))
				{
					// extract the transactions list
					QList<QMap<QString, QString>> transactions;
					QJsonArray					  transactionsArray = dataObject.value("List").toArray();

					for (int i = 0; i < transactionsArray.size(); i++)
					{
						QMap<QString, QString> transaction;
						QJsonObject			   transactionObject = transactionsArray.at(i).toObject();

						transaction.insert("from_account_number",
										   QString::number(transactionObject.value("from_account_number").toInt()));
						transaction.insert("to_account_number",
										   QString::number(transactionObject.value("to_account_number").toInt()));
						transaction.insert("transaction_amount",
										   QString::number(transactionObject.value("amount").toDouble(), 'f', 2));
						transaction.insert("created_at", transactionObject.value("created_at").toString());

						transactions.append(transaction);
					}

					emit TransactionsFetched(transactions);
				}
				else
				{
					//emit FailedRequestUserWidget(getResponseMessage(dataObject));
					emit FailedRequest(getResponseMessage(dataObject));
				}
				break;

			case MakeTransaction:
				if (getResponseStatus(dataObject))
				{
					//emit SuccessfullRequestUserWidget(getResponseMessage(dataObject));
					emit SuccessfullRequest(getResponseMessage(dataObject));
				}
				else
				{
					//emit FailedRequestUserWidget(getResponseMessage(dataObject));
					emit FailedRequest(getResponseMessage(dataObject));
				}

				break;

			case TransferAmount:

				// handle the transfer amount response
				break;

			case GetDatabase:
				if (getResponseStatus(dataObject))
				{
					// extract the database content
					QList<QMap<QString, QString>> databaseContent;
					QJsonArray					  databaseArray = dataObject.value("users").toArray();

					for (int i = 0; i < databaseArray.size(); i++)
					{
						QMap<QString, QString> user;
						QJsonObject			   userObject = databaseArray.at(i).toObject();

						user.insert("first_name", userObject.value("first_name").toString());
						user.insert("last_name", userObject.value("last_name").toString());
						user.insert("email", userObject.value("email").toString());
						user.insert("role", userObject.value("role").toString());
						user.insert("account_number", QString::number(userObject.value("account_number").toInt()));
						user.insert("balance", QString::number(userObject.value("balance").toDouble(), 'f', 2));

						databaseContent.append(user);
					}

					emit DatabaseFetched(databaseContent);
				}
				else
				{
					//emit FailedRequestUserWidget(getResponseMessage(dataObject));
					emit FailedRequest(getResponseMessage(dataObject));
				}

				break;

			case CreateNewUser:
				if (getResponseStatus(dataObject))
				{
					//emit SuccessfullRequestUserWidget(getResponseMessage(dataObject));
					emit SuccessfullRequest(getResponseMessage(dataObject));
				}
				else
				{
					//emit FailedRequestUserWidget(getResponseMessage(dataObject));
					emit FailedRequest(getResponseMessage(dataObject));
				}

				break;

			case DeleteUser:
				if (getResponseStatus(dataObject))
				{
					//emit SuccessfullRequestUserWidget(getResponseMessage(dataObject));
					emit SuccessfullRequest(getResponseMessage(dataObject));
				}
				else
				{
					//emit FailedRequestUserWidget(getResponseMessage(dataObject));
					emit FailedRequest(getResponseMessage(dataObject));
				}

				break;

			case UpdateUser:
				if (getResponseStatus(dataObject))
				{
					//emit SuccessfullRequestUserWidget(getResponseMessage(dataObject));
					emit SuccessfullRequest(getResponseMessage(dataObject));
				}
				else
				{
					//emit FailedRequestUserWidget(getResponseMessage(dataObject));
					emit FailedRequest(getResponseMessage(dataObject));
				}
				// handle the update user response
				break;

			case UserInit:
				if (getResponseStatus(dataObject))
				{
					// get user name and role
					QString first_name = dataObject.value("first_name").toString();
					QString role = dataObject.value("role").toString();
					QString email = dataObject.value("email").toString();

					if (role == "admin")
					{
						emit adminLoginSuccess(email, first_name);
					}
					else if (role == "user")
					{
						// get account number, balance and transactions
						QString account_number = QString::number(dataObject.value("account_number").toInt());

						QString balance = QString::number(dataObject.value("current_balance").toDouble(), 'f', 2);

						emit userloginSuccess(email, first_name, account_number, balance);
					}
				}
				else
				{
					emit loginFailure("Something went wrong! Please try again.");
				}
				break;
			case UpdateEmail:
				if (getResponseStatus(dataObject))
				{
					//emit SuccessfullRequestUserWidget(getResponseMessage(dataObject));
					emit SuccessfullRequest(getResponseMessage(dataObject));
				}
				else
				{
					//emit FailedRequestUserWidget(getResponseMessage(dataObject));
					emit FailedRequest(getResponseMessage(dataObject));
				}
				break;
			case UpdatePassword:
				if (getResponseStatus(dataObject))
				{
					//emit SuccessfullRequestUserWidget(getResponseMessage(dataObject));
					emit SuccessfullRequest(getResponseMessage(dataObject));
				}
				else
				{
					//emit FailedRequestUserWidget(getResponseMessage(dataObject));
					emit FailedRequest(getResponseMessage(dataObject));
				}
				break;
			case JsonParseError:
				if (getResponseStatus(dataObject))
				{
					//emit SuccessfullRequestUserWidget(getResponseMessage(dataObject));
					emit SuccessfullRequest(getResponseMessage(dataObject));
				}
				else
				{
					//emit FailedRequestUserWidget(getResponseMessage(dataObject));
					emit FailedRequest(getResponseMessage(dataObject));
				}

				// handle the json parse error response
				break;
		}

		return status;
	}
};

#endif // RESPONSEMANAGER_H
