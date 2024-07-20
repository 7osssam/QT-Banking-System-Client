#include <gtest/gtest.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSignalSpy>

#include <QDebug>

#include <QList>
#include <QVariant>

#include "ResponseManager.h"

// Test Fixture
class ResponseManagerTest : public ::testing::Test
{
protected:
	ResponseManager* responseManager;

	void SetUp() override
	{
		responseManager = new ResponseManager();
	}

	void TearDown() override
	{
		delete responseManager;
	}
};

// Test for getResponseStatus
TEST_F(ResponseManagerTest, GetResponseStatus_ValidStatus_ReturnsTrue)
{
	QJsonObject data;
	data.insert("status", 1);

	EXPECT_TRUE(responseManager->getResponseStatus(data));
}

TEST_F(ResponseManagerTest, GetResponseStatus_InvalidStatus_ReturnsFalse)
{
	QJsonObject data;
	data.insert("status", 0);

	EXPECT_FALSE(responseManager->getResponseStatus(data));
}

TEST_F(ResponseManagerTest, GetResponseStatus_NoStatus_ReturnsFalse)
{
	QJsonObject data;

	EXPECT_FALSE(responseManager->getResponseStatus(data));
}

// Test for getResponseMessage
TEST_F(ResponseManagerTest, GetResponseMessage_ValidMessage_ReturnsMessage)
{
	QJsonObject data;
	data.insert("message", "Test message");

	EXPECT_EQ(responseManager->getResponseMessage(data), "Test message");
}

TEST_F(ResponseManagerTest, GetResponseMessage_NoMessage_ReturnsEmpty)
{
	QJsonObject data;

	EXPECT_EQ(responseManager->getResponseMessage(data), "");
}

// Test for handleResponse
TEST_F(ResponseManagerTest, HandleResponse_Connection_Successful)
{
	QSignalSpy connectionSpy(responseManager, &ResponseManager::ConnectionResponse);

	QJsonObject data;
	data.insert("Response", -2);
	QJsonObject dataObject;
	dataObject.insert("status", 1);
	data.insert("Data", dataObject);

	responseManager->handleResponse(data);

	QList<QVariant> fetchedSignals = connectionSpy.takeFirst();
	EXPECT_EQ(fetchedSignals.first().toBool(), true);
}

TEST_F(ResponseManagerTest, HandleResponse_Login_Failed)
{
	QSignalSpy failedRequestSpy(responseManager, &ResponseManager::FailedRequest);

	QJsonObject data;
	data.insert("Response", 1);
	QJsonObject dataObject;
	dataObject.insert("status", 0);
	dataObject.insert("message", "Login failed");
	data.insert("Data", dataObject);

	responseManager->handleResponse(data);

	QList<QVariant> fetchedSignals = failedRequestSpy.takeFirst();
	qDebug() << fetchedSignals.first().toString();
	EXPECT_EQ(fetchedSignals.first().toString(), "Login Failed : Login failed");
}

// Add more tests for different response codes as needed...

// Example for handling signals
TEST_F(ResponseManagerTest, HandleResponse_Successful_FetchBalance)
{
	QSignalSpy balanceFetchedSpy(responseManager, &ResponseManager::BalanceFetched);

	QJsonObject data;
	data.insert("Response", 3); // Assuming 4 is the code for GetBalance
	QJsonObject dataObject;
	dataObject.insert("status", 1);
	dataObject.insert("balance", 123.45);
	data.insert("Data", dataObject);

	responseManager->handleResponse(data);

	QList<QVariant> fetchedSignals = balanceFetchedSpy.takeFirst();
	EXPECT_EQ(fetchedSignals.first().toString(), "123.45");
}
