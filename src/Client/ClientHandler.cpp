/**
 * @file clienthandler.cpp
 * @brief Implementation file for the ClientHandler class.
 *
 * This file contains the implementation of the ClientHandler class, which is responsible
 * for managing client requests and responses using the TcpClient class.
 */

#include "clienthandler.h"
#include <QDebug>

ClientHandler::ClientHandler(QObject* parent) : QObject(parent), tcpClient(nullptr)
{
}

ClientHandler::~ClientHandler()
{
	qDebug() << "ClientHandler destroyed";
}

void ClientHandler::run()
{
	QEventLoop loop;

	tcpClient = new TcpClient(this);
	connect(tcpClient, &TcpClient::ResponseReadySignal, this, &ClientHandler::onResponseReady);
	connect(tcpClient, &TcpClient::ConnectedSignal, this, &ClientHandler::onConnectedSignal);
	connect(tcpClient, &TcpClient::DisconnectedSignal, this, &ClientHandler::onDisconnectedSignal);
	loop.exec();
}

void ClientHandler::sendRequest(QJsonObject request)
{
	QJsonDocument jsonDoc(request);
	QByteArray	  jsonByteArray = jsonDoc.toJson();
	tcpClient->sendTcpRequest(jsonByteArray);
}

void ClientHandler::onResponseReady(QByteArray response)
{
	QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
	QJsonObject	  jsonObject = jsonResponse.object();

	emit sendResponseBack(jsonObject);
}

void ClientHandler::requestClientConnection(const QString& host, quint16 port)
{
	tcpClient->connectToServer(host, port);
}

void ClientHandler::requestClientDisconnection()
{
	tcpClient->closeConnection();
	qInfo() << "Disconnecting on thread" << QThread::currentThreadId();
}

void ClientHandler::onConnectedSignal()
{
	QJsonObject response;
	response.insert("Response", -2);
	QJsonObject data;
	data.insert("status", 1);
	response.insert("Data", data);

	emit sendResponseBack(response);
}

void ClientHandler::onDisconnectedSignal()
{
	QJsonObject response;
	response.insert("Response", -2);
	QJsonObject data;
	data.insert("status", 0);
	response.insert("Data", data);

	emit sendResponseBack(response);
}
