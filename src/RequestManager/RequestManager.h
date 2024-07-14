#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

// this class is responsible for managing the requests sent to the server
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class RequestManager : public QObject
{
	Q_OBJECT
private:
	explicit RequestManager(QObject* parent = nullptr) : QObject(parent)
	{
	}

signals:
	void makeRequest(QJsonObject Data);

public:
	RequestManager(const RequestManager&) = delete;
	void				   operator=(const RequestManager&) = delete;
	static RequestManager* getInstance(QObject* parent = nullptr)
	{
		static RequestManager* instance = new RequestManager(parent);
		return instance;
	}

	void sendLoginRequest(const QString& email, const QString& password)
	{
		QJsonObject loginData;
		loginData.insert("Request", 1);
		QJsonObject data;
		data.insert("email", email);
		data.insert("password", password);

		loginData.insert("Data", data);

		emit makeRequest(loginData);
	}

	// TODO: get account number (2)

	void sendGetBalanceRequest(const int& account_number)
	{
		QJsonObject getBalanceData;
		getBalanceData.insert("Request", 3);
		QJsonObject data;
		data.insert("account_number", account_number);

		getBalanceData.insert("Data", data);

		emit makeRequest(getBalanceData);
	}

	void sendGetTransactionsHistoryRequest(const QString& email)
	{
		QJsonObject getTransactionsHistoryData;
		getTransactionsHistoryData.insert("Request", 4);
		QJsonObject data;
		data.insert("email", email);

		getTransactionsHistoryData.insert("Data", data);

		emit makeRequest(getTransactionsHistoryData);
	}

	void sendTransferRequest(const QString& from_account_number, const QString& receiver,
							 const double& transaction_amount, bool is_email)
	{
		QJsonObject transferData;
		transferData.insert("Request", 5);
		QJsonObject data;
		data.insert("from_account_number", from_account_number.toInt());
		if (is_email)
		{
			data.insert("to_account_number", -1);
			data.insert("to_email", receiver);
		}
		else
		{
			data.insert("to_account_number", receiver.toInt());
			data.insert("to_email", "");
		}
		data.insert("transaction_amount", transaction_amount);

		transferData.insert("Data", data);

		emit makeRequest(transferData);
	}

	// TODO: Transfer amount (6)

	void sendGetDatabaseContentRequest(const QString& email)
	{
		QJsonObject getDatabaseContentData;
		getDatabaseContentData.insert("Request", 7);
		QJsonObject data;
		data.insert("email", email);

		getDatabaseContentData.insert("Data", data);

		emit makeRequest(getDatabaseContentData);
	}

	void sendCreateNewUserRequest(const QString& email, QMap<QString, QString> new_data)
	{
		QJsonObject createNewUserData;
		createNewUserData.insert("Request", 8);
		QJsonObject data;
		data.insert("email", email);

		QJsonArray	newUser;
		QJsonObject user;
		user.insert("first_name", new_data["first_name"]);
		user.insert("last_name", new_data["last_name"]);
		user.insert("email", new_data["email"]);
		user.insert("password", new_data["password"]);
		user.insert("role", new_data["role"]);
		user.insert("initial_balance", new_data["initial_balance"].toDouble());
		newUser.append(user);

		data.insert("newUser", newUser);
		createNewUserData.insert("Data", data);

		emit makeRequest(createNewUserData);
	}

	void sendDeleteUserRequest(const QString& email, const QString& account_number)
	{
		QJsonObject deleteUserRequest;
		deleteUserRequest.insert("Request", 9);
		QJsonObject data;
		data.insert("email", email);
		data.insert("account_number", account_number.toInt());

		deleteUserRequest.insert("Data", data);

		emit makeRequest(deleteUserRequest);
	}

	void sendUpdateUserRequest(const QString& email, const QString& account_number, QMap<QString, QString> new_data)
	{
		QJsonObject updateUserRequest;
		updateUserRequest.insert("Request", 10);
		QJsonObject data;
		data.insert("email", email);
		data.insert("account_number", account_number.toInt());

		QJsonObject newUser;
		newUser.insert("first_name", new_data["first_name"]);
		newUser.insert("last_name", new_data["last_name"]);
		newUser.insert("email", new_data["email"]);
		newUser.insert("role", new_data["role"]);
		data.insert("newData", newUser);

		updateUserRequest.insert("Data", data);

		emit makeRequest(updateUserRequest);
	}

	void sendinitRequest(const QString& email, const QString& password)
	{
		QJsonObject initRequest;
		initRequest.insert("Request", 11);
		QJsonObject data;
		data.insert("email", email);
		data.insert("password", password);

		initRequest.insert("Data", data);

		emit makeRequest(initRequest);
	}

	void sendUpdateEmailRequest(const QString& email, const QString& password, const QString& new_email)
	{
		QJsonObject updateEmailData;
		updateEmailData.insert("Request", 12);
		QJsonObject data;
		data.insert("email", email);
		data.insert("password", password);
		data.insert("new_email", new_email);

		updateEmailData.insert("Data", data);

		emit makeRequest(updateEmailData);
	}

	void sendUpdatePasswordRequest(const QString& email, const QString& password, const QString& new_password)
	{
		QJsonObject updatePasswordData;
		updatePasswordData.insert("Request", 13);
		QJsonObject data;
		data.insert("email", email);
		data.insert("password", password);
		data.insert("new_password", new_password);

		updatePasswordData.insert("Data", data);

		emit makeRequest(updatePasswordData);
	}
};

#endif // REQUESTMANAGER_H
