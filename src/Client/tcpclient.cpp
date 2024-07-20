/**
 * @file tcpclient.cpp
 * @brief Implementation file for the TcpClient class.
 *
 * This file contains the implementation of the TcpClient class methods.
 */
#include "tcpclient.h"

#include <QMetaEnum>
#include <QEventLoop>

#include <QCryptographicHash>
#include <QByteArray>

#include <QFile>
#include <QSslCertificate>
#include <QSslConfiguration>

#include <QOverload>

TcpClient::TcpClient(QObject* parent) : QObject(parent)
{
	socket = new QTcpSocket(this);

	connect(socket, &QSslSocket::connected, this, &TcpClient::onConnected);
	connect(socket, &QSslSocket::readyRead, this, &TcpClient::onReadyRead);

	connect(socket, &QSslSocket::stateChanged, this, &TcpClient::onStateChanged);
	connect(socket, &QSslSocket::errorOccurred, this, &TcpClient::onErrorOccurred);

	connect(socket, &QSslSocket::disconnected, this, &TcpClient::onDisconnected);
}

TcpClient::~TcpClient()
{
	delete socket;
}

void TcpClient::connectToServer(const QString& host, quint16 port)
{
	qDebug() << "Connecting to server " << host << " on port " << port;

	socket->connectToHost(host, port);
}

void TcpClient::sendTcpRequest(const QByteArray& request)
{
	if (socket->state() == QAbstractSocket::ConnectedState)
	{
		// Generate hash of the request
		QCryptographicHash hash(QCryptographicHash::Sha256);

		hash.addData(request);
		QByteArray hashValue = hash.result();

		// Append hash to the request data
		QByteArray dataToSend = request + hashValue;

		// Send the data
		socket->write(dataToSend);
		socket->waitForBytesWritten();
	}
}

void TcpClient::closeConnection()
{
	if (socket->isOpen())
	{
		socket->disconnectFromHost();
		socket->waitForDisconnected(500);
	}
}

void TcpClient::onConnected()
{
	qDebug() << "Connected to server";
}

void TcpClient::onReadyRead()
{
	QByteArray response = socket->readAll();

	emit ResponseReadySignal(response);
}

void TcpClient::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
	QMetaEnum metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketError>();
	qDebug() << "Error occurred: " << metaEnum.valueToKey(socketError);
}

void TcpClient::onStateChanged(QAbstractSocket::SocketState socketState)
{
	QMetaEnum metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketState>();
	qDebug() << "State changed: " << metaEnum.valueToKey(socketState);

	if (socketState == QAbstractSocket::UnconnectedState)
	{
		emit DisconnectedSignal();
	}
	else if (socketState == QAbstractSocket::ConnectedState)
	{
		emit ConnectedSignal();
	}
}

void TcpClient::onDisconnected()
{
	qDebug() << "Disconnected from server";
}