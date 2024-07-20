/**
 * @file UserWidget.h
 * @brief Header file for the UserWidget class.
 *
 * The UserWidget class provides a user interface for managing user accounts, and performing transactions within the banking application.
 */

#ifndef USERWIDGET_H
#define USERWIDGET_H

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

#include "RequestManager.h"

/**
 * @class UserWidget
 * @brief The UserWidget class provides a user interface for managing user accounts,
 * performing transactions, and modifying settings within the banking application.
 *
 * This class represents the main user interface component that allows users to interact with their
 * account information, view transaction history, perform transfers, and update personal settings.
 * It includes several tabs for different functionalities such as Home, Transfer, and Settings.
 *
 * @details The UserWidget class manages the display of user data, handles user interactions, and communicates
 * with the backend server through the RequestManager. It provides features for viewing transaction history,
 * performing money transfers, updating account details, and logging out of the application.
 *
 * The widget consists of:
 * - Home tab: Displays welcome message, current balance, and transaction history.
 * - Transfer tab: Allows users to transfer funds to another account or email.
 * - Settings tab: Provides options to update email, update password, and log out.
 */
class UserWidget : public QWidget
{
	Q_OBJECT

public:
	/**
     * @brief Constructs a UserWidget instance.
     * @param email The user's email address.
     * @param first_name The user's first name.
     * @param account_number The user's account number.
     * @param balance The user's current balance.
     * @param parent The parent widget (optional).
     */
	UserWidget(QString email, QString first_name, QString account_number, QString balance, QWidget* parent = nullptr);

signals:
	/**
     * @brief Emitted when the user requests to log out.
     */
	void logout();

public slots:
	/**
     * @brief Slot to handle successful fetch of transactions.
     * @param transactions The list of transactions fetched from the server.
     */
	void onTransactionsFetched(const QList<QMap<QString, QString>>& transactions);

	/**
     * @brief Slot to handle successful fetch of balance.
     * @param balance The balance fetched from the server.
     */
	void onBalanceFetched(const QString balance);

	/**
     * @brief Slot to handle successful request.
     * @param message The success message to display.
     */
	void onSuccessfullRequest(QString message);

	/**
     * @brief Slot to handle failed request.
     * @param message The failure message to display.
     */
	void onFailedRequest(QString message);

private slots:
	/**
     * @brief Slot to handle email update request.
     */
	void onUpdateEmail();

	/**
     * @brief Slot to handle password update request.
     */
	void onUpdatePassword();

	/**
     * @brief Slot to handle logout button click.
     */
	void onLogoutClicked();

	/**
     * @brief Slot to handle logout confirmation.
     */
	void onLogoutConfirmed();

	/**
     * @brief Updates the transactions table with fetched data.
     */
	void updateTransactionsTable();

	/**
     * @brief Slot to handle balance label click.
     */
	void onBalanceLabelClicked();

	/**
     * @brief Updates the welcome label with the user's first name and account number.
     */
	void updateFirstNameLabel();

	/**
     * @brief Slot to handle changes in transfer input fields.
     */
	void onTransferFieldsChanged();

	/**
     * @brief Slot to handle transfer button click.
     */
	void onTransferButtonClicked();

private:
	/**
     * @brief Creates the Home tab widget.
     * @return The Home tab widget.
     */
	QWidget* createHomeTab();

	/**
     * @brief Creates the Transfer tab widget.
     * @return The Transfer tab widget.
     */
	QWidget* createTransferTab();

	/**
     * @brief Creates the Settings tab widget.
     * @return The Settings tab widget.
     */
	QWidget* createSettingsTab();

	/**
     * @brief Creates a vertical layout for tabs.
     * @return The vertical layout.
     */
	QVBoxLayout* createTabLayout();

	QString						  email_;		   ///< The user's email address.
	QString						  first_name_;	   ///< The user's first name.
	QString						  account_number_; ///< The user's account number.
	QString						  balance_;		   ///< The user's current balance.
	QList<QMap<QString, QString>> transactions_;   ///< List of transactions.

	RequestManager* requestManager;				   ///< The request manager for communication with the server.

	QtMaterialSnackbar*	  notificationSnackbar;	   ///< Snackbar for notifications.
	QtMaterialTabs*		  tabs;					   ///< Tab widget for navigating between different sections.
	QStackedWidget*		  tabContents;			   ///< Stacked widget for displaying the contents of each tab.
	QtMaterialDialog*	  logoutDialog;			   ///< Dialog for confirming logout.
	QtMaterialFlatButton* balanceLabel;			   ///< Button displaying the current balance.
	QtMaterialFlatButton* welcomeLabel;			   ///< Button displaying the welcome message.

	QtMaterialTextField*  toAccountField;		   ///< Text field for entering the account number to transfer to.
	QtMaterialTextField*  toEmailField;			   ///< Text field for entering the email address to transfer to.
	QtMaterialTextField*  amountField;			   ///< Text field for entering the amount to transfer.
	QtMaterialFlatButton* transferButton;		   ///< Button to initiate the transfer.
	QTableWidget*		  transactionsTable;	   ///< Table displaying transaction history.
};

#endif											   // USERWIDGET_H
