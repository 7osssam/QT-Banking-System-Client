/**
 * @file clienthandler.h
 * @brief Header file for the ClientHandler class.
 *
 * This file contains the declaration of the ClientHandler class, which is responsible
 * for managing client requests and responses using the @ref TcpClient class.
 */

#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>
#include "tcpclient.h"

/**
 * @class ClientHandler
 * @brief A class for managing client requests and responses.
 *
 * The ClientHandler class handles the client-side operations such as sending requests,
 * receiving responses, and managing connections. It uses the TcpClient class to handle
 * the actual TCP communication.
 */
class ClientHandler : public QObject
{
	Q_OBJECT
public:
	/**
     * @brief Constructor for ClientHandler.
     * @param parent The parent QObject, default is nullptr.
     */
	explicit ClientHandler(QObject* parent = nullptr);

	/**
     * @brief Destructor for ClientHandler.
     */
	~ClientHandler();

	/**
     * @brief Runs the event loop for the ClientHandler.
     */
	void run();

public slots:
	/**
     * @brief Sends a request to the server.
     * @param request The JSON object containing the request data.
     */
	void sendRequest(QJsonObject request);

	/**
     * @brief Slot to handle the response received from the server.
     * @param response The response data in QByteArray format.
     */
	void onResponseReady(QByteArray response);

	/**
     * @brief Requests a connection to the server.
     * @param host The server host.
     * @param port The server port.
     */
	void requestClientConnection(const QString& host, quint16 port);

	/**
     * @brief Requests disconnection from the server.
     */
	void requestClientDisconnection();

	/**
     * @brief Slot to handle the connected signal.
     */
	void onConnectedSignal();

	/**
     * @brief Slot to handle the disconnected signal.
     */
	void onDisconnectedSignal();

signals:
	/**
     * @brief Signal to send the response back to the window manager.
     * @param response The JSON object containing the response data.
     */
	void sendResponseBack(QJsonObject response);

private:
	TcpClient* tcpClient; ///< Pointer to the TcpClient instance.
};

#endif					  // CLIENTHANDLER_H
