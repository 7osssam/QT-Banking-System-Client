#include "RequestManager.h"

RequestManager::RequestManager(QObject* parent) : QObject(parent)
{
}

RequestManager* RequestManager::getInstance(QObject* parent)
{
	static RequestManager* instance = new RequestManager(parent);
	return instance;
}

void RequestManager::createRequest(AvailableRequests requestType, QVariantMap data)
{
	QJsonObject request;
	request.insert("Request", requestType);
	QJsonObject requestData;

	for (auto it = data.constBegin(); it != data.constEnd(); ++it)
	{
		requestData.insert(it.key(), QJsonValue::fromVariant(it.value()));
	}

	request.insert("Data", requestData);

	emit makeRequest(request);
}