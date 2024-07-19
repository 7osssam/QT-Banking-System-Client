
#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <QObject>
#include <QThread>
#include "UIManager.h"
#include "ClientHandler.h"

/**
 * @file AppManager.h
 * @brief Manages the application's UI and client handling in separate threads.
 * @details This class is responsible for coordinating between the UI and the client handler,
 * ensuring that the application remains responsive by running the client handling logic in a separate thread.
 */
class AppManager : public QObject
{
	Q_OBJECT

private:
	UIManager* uiManager;	/**< Pointer to the UIManager instance, which handles UI-related tasks. */
	ClientHandler*
			 clientHandler; /**< Pointer to the ClientHandler instance, which handles communication with the server. */
	QThread* AppManagerThread; /**< Thread to run the ClientHandler, keeping the main GUI responsive. */

	/**
     * @brief Private constructor for the AppManager class.
     * @details This constructor initializes the UIManager, ClientHandler, and sets up the necessary signal-slot connections.
     * @param parent Optional parent QObject.
     */
	explicit AppManager(QObject* parent = nullptr);

public:
	/**
     * @brief Starts the AppManager thread.
     * @details This function starts the QThread which runs the ClientHandler.
     */
	void start();

	/**
     * @brief Gets the singleton instance of AppManager.
     * @param parent Optional parent QObject.
     * @return Reference to the AppManager instance.
     */
	static AppManager& getInstance(QObject* parent = nullptr);

	/**
     * @brief Deleted copy constructor to prevent copying.
     */
	AppManager(const AppManager&) = delete;

	/**
     * @brief Deleted assignment operator to prevent copying.
     */
	void operator=(const AppManager&) = delete;

	/**
     * @brief Destructor for the AppManager class.
     * @details Cleans up the UIManager and ClientHandler instances.
     */
	~AppManager();
};

#endif // APPMANAGER_H
