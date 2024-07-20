#include "ResponseManager.h"

bool ResponseManager::getResponseStatus(QJsonObject Data)
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

QString ResponseManager::getResponseMessage(QJsonObject Data)
{
	if (Data.contains("message"))
	{
		return Data.value("message").toString();
	}
	return "";
}

ResponseManager::ResponseManager(QObject* parent) : QObject(parent)
{
}

void ResponseManager::handleResponse(QJsonObject Data)
{
	int			responseCode = 0; // make sure that there is no response code = 0
	QJsonObject dataObject;

	qDebug().noquote() << "---> Response received from server:\n"
					   << QJsonDocument(Data).toJson(QJsonDocument::Indented);

	// get the response code
	if (Data.contains("Response"))
	{
		responseCode = Data.value("Response").toInt();
	}
	else
	{
		responseCode = JsonParseError;
	}

	// check for the response data object
	if (Data.contains("Data"))
	{
		dataObject = Data.value("Data").toObject();
	}
	else
	{
		responseCode = JsonParseError;
	}

	switch (responseCode)
	{
		case Connection:
			if (getResponseStatus(dataObject))
			{
				emit ConnectionResponse(true);
			}
			else
			{
				emit ConnectionResponse(false);
			}
			break;

		case Login:
			if (getResponseStatus(dataObject))
			{
				QString first_name = dataObject.value("first_name").toString();
				QString role = dataObject.value("role").toString();

				emit SuccessfullRequest("Login Successfull : " + role);
			}
			else
			{
				emit FailedRequest("Login Failed : " + getResponseMessage(dataObject));
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
				emit FailedRequest(getResponseMessage(dataObject));
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
				emit FailedRequest(getResponseMessage(dataObject));
			}
			break;

		case TransferAmount: // not emplemented

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
				emit FailedRequest(getResponseMessage(dataObject));
			}

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
				emit FailedRequest("Something went wrong! Please try again.");
			}
			break;
		case UpdateEmail:
		case UpdateUser:
		case DeleteUser:
		case CreateNewUser:
		case MakeTransaction:
		case UpdatePassword:
		case JsonParseError:
			if (getResponseStatus(dataObject))
			{
				emit SuccessfullRequest(getResponseMessage(dataObject));
			}
			else
			{
				emit FailedRequest(getResponseMessage(dataObject));
			}
			break;

		default:
			qDebug() << "Unknown response code: " << responseCode;
			emit FailedRequest("Unknown response code: " + QString::number(responseCode));

			break;
	}
}