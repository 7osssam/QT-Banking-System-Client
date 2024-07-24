/**
 * @file AdminWidget.h
 * @brief Header file for the AdminWidget class.
 *
 * The AdminWidget class provides a user interface for administrators to manage users, view transactions, and update settings.
 * It features tabs for managing the database, transactions, and settings, as well as floating action buttons for user management tasks.
 */

#ifndef ADMINWIDGET_H
#define ADMINWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QStackedWidget>
#include <QTableWidget>
#include "qtmaterialtabs.h"
#include "qtmaterialflatbutton.h"
#include "qtmaterialdialog.h"
#include "qtmaterialsnackbar.h"
#include "qtmaterialtextfield.h"
#include "qtmaterialfab.h"
#include "RequestManager.h"

#include <QVariantMap>


/**
 * @class AdminWidget
 * @brief Provides an interface for administrative tasks within the banking system.
 *
 * The AdminWidget class allows administrators to manage user accounts, view transaction history, and update settings.
 * It includes tabs for managing the database, viewing transactions, and updating settings, as well as floating action buttons for user management tasks.
 * The widget provides features for viewing database content, managing user accounts, and logging out of the application.
 * The AdminWidget communicates with the backend server through the RequestManager to perform various operations.
 *
 * The widget consists of:
 * - Database tab: Displays database content and allows users.
 * - Transactions tab: Shows transaction history for all bank accounts (from -> to -> amount).
 * - Settings tab: Allows the admin to update their email address and password.
 */

class AdminWidget : public QWidget
{
	Q_OBJECT

public:
	/**
     * @brief Constructs an AdminWidget object.
     *
     * @param email The email of the admin.
     * @param first_name The first name of the admin.
     * @param parent The parent widget (default is nullptr).
     */
	AdminWidget(QString email, QString first_name, QWidget* parent = nullptr);

signals:
	/**
     * @brief Signal emitted when the admin logs out.
     */
	void logout();

public slots:
	/**
     * @brief Slot for handling updates to the database content.
     *
     * @param data The updated database content.
     */
	void onDatabaseContentUpdated(const QList<QMap<QString, QString>>& data);

	/**
     * @brief Slot for handling fetched transaction history.
     *
     * @param transactions The fetched transactions.
     */
	void onTransactionsFetched(const QList<QMap<QString, QString>>& transactions);

	/**
     * @brief Slot for handling successful request messages.
     *
     * @param message The success message.
     */
	void onSuccessfullRequest(QString message);

	/**
     * @brief Slot for handling failed request messages.
     *
     * @param message The failure message.
     */
	void onFailedRequest(QString message);

private slots:
	/**
     * @brief Slot for updating the admin's email address.
     */
	void onUpdateEmail();

	/**
     * @brief Slot for updating the admin's password.
     */
	void onUpdatePassword();

	/**
     * @brief Slot for handling the logout button click.
     */
	void onLogoutClicked();

	/**
     * @brief Slot for confirming the logout action.
     */
	void onLogoutConfirmed();

	/**
     * @brief Slot for updating the database table view.
     */
	void updateDatabaseTable();

	/**
     * @brief Slot for updating the transactions table view.
     */
	void updateTransactionsTable();

	/**
     * @brief Slot for handling the update user button click.
     */
	void onUpdateUserClicked();

	/**
     * @brief Slot for handling the delete user button click.
     */
	void onDeleteUserClicked();

	/**
     * @brief Slot for handling the create new user button click.
     */
	void onCreateNewUserClicked();

	/**
     * @brief Slot for handling user selection changes in the database table.
     */
	void onUserSelectionChanged();

private:
	/**
     * @brief Creates the widget for the database management tab.
     *
     * @return A QWidget pointer to the database tab.
     */
	QWidget* createDatabaseTab();

	/**
     * @brief Creates the widget for the transactions history tab.
     *
     * @return A QWidget pointer to the transactions tab.
     */
	QWidget* createTransactionsTab();

	/**
     * @brief Creates the widget for the settings tab.
     *
     * @return A QWidget pointer to the settings tab.
     */
	QWidget* createSettingsTab();

	/**
     * @brief Creates a layout for a tab.
     *
     * @return A QVBoxLayout pointer to the created layout.
     */
	QVBoxLayout* createTabLayout();

	/**
     * @brief Sets up floating action buttons for user management.
     */
	void setupFloatingActionButtons();

	QString						  admin_email_;		  ///< The email of the admin.
	QString						  admin_new_email_;	  ///< The potential new email of the admin.
	QString						  admin_first_name_;  ///< The first name of the admin.
	QList<QMap<QString, QString>> transactions_;	  ///< List of transactions.
	QList<QMap<QString, QString>> databaseContent_;	  ///< List of database content.
	RequestManager*				  requestManager;	  ///< The request manager for handling server requests.

	QtMaterialFlatButton* welcomeLabel;				  ///< Welcome label showing admin's first name.
	QtMaterialSnackbar*	  notificationSnackbar;		  ///< Snackbar for displaying messages.
	QtMaterialTabs*		  tabs;						  ///< Tabs widget for switching between different views.
	QStackedWidget*		  tabContents;				  ///< Stacked widget to hold tab content.
	QtMaterialDialog*	  logoutDialog;				  ///< Dialog for confirming logout.

	QTableWidget* databaseTable;					  ///< Table widget for displaying database content.
	QTableWidget* transactionsTable;				  ///< Table widget for displaying transactions.

	QtMaterialFloatingActionButton* updateUserFab;	  ///< Floating action button for updating a user.
	QtMaterialFloatingActionButton* deleteUserFab;	  ///< Floating action button for deleting a user.
	QtMaterialFloatingActionButton* createNewUserFab; ///< Floating action button for creating a new user.

	QVariantMap selectedUserData;					  ///< Data of the currently selected user.
};

#endif												  // ADMINWIDGET_H
