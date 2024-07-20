/**
 * @file tcpclient.h
 * @brief Header file for the TcpClient class.
 *
 * This file contains the declaration of the TcpClient class, which is responsible
 * for managing TCP connections, sending requests, and receiving responses in the application.
 */

#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QSslSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

/**
 * @class TcpClient
 * @brief A class for managing TCP client connections.
 *
 * The TcpClient class is responsible for establishing a connection to a TCP server,
 * sending requests, receiving responses, and handling various socket states and errors.
 * It provides signals for connection status and incoming responses.
 */
class TcpClient : public QObject
{
	Q_OBJECT
public:
	/**
     * @brief Constructor for TcpClient.
     * @param parent The parent object.
     *
     * Initializes the TcpClient object and connects the necessary signals and slots.
     */
	explicit TcpClient(QObject* parent = nullptr);

	/**
     * @brief Destructor for TcpClient.
     *
     * Cleans up the TcpClient object, ensuring the socket is properly deleted.
     */
	~TcpClient();

	/**
     * @brief Connect to a TCP server.
     * @param host The host address of the server.
     * @param port The port number to connect to.
     *
     * Initiates a connection to the specified server.
     */
	void connectToServer(const QString& host, quint16 port);

	/**
     * @brief Send a TCP request.
     * @param request The request data to be sent.
     *
     * Sends the specified request to the connected server.
     */
	void sendTcpRequest(const QByteArray& request);

	/**
     * @brief Close the TCP connection.
     *
     * Closes the connection to the server if it is open.
     */
	void closeConnection();

signals:
	/**
     * @brief Signal emitted when a response is ready.
     * @param response The response data received from the server.
     */
	void ResponseReadySignal(QByteArray response);

	/**
     * @brief Signal emitted when connected to the server.
     */
	void ConnectedSignal();

	/**
     * @brief Signal emitted when disconnected from the server.
     */
	void DisconnectedSignal();

private slots:
	/**
     * @brief Slot for handling the connected state.
     *
     * Called when the socket successfully connects to the server.
     */
	void onConnected();

	/**
     * @brief Slot for handling incoming data.
     *
     * Called when data is available to read from the socket.
     */
	void onReadyRead();

	/**
     * @brief Slot for handling socket errors.
     * @param socketError The type of socket error that occurred.
     *
     * Called when an error occurs with the socket.
     */
	void onErrorOccurred(QAbstractSocket::SocketError socketError);

	/**
     * @brief Slot for handling socket state changes.
     * @param socketState The new state of the socket.
     *
     * Called when the state of the socket changes.
     */
	void onStateChanged(QAbstractSocket::SocketState socketState);

	/**
     * @brief Slot for handling the disconnected state.
     *
     * Called when the socket disconnects from the server.
     */
	void onDisconnected();

private:
	QTcpSocket* socket; ///< The TCP socket used for communication.
};

#endif // TCPCLIENT_H