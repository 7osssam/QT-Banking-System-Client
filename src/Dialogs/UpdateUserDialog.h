/**
 * @file UpdateUserDialog.h
 * @brief Header file for the UpdateUserDialog class.
 * @details Declares the UpdateUserDialog class which provides a dialog for updating user information with input validation.
**/

#ifndef UPDATEUSERDIALOG_H
#define UPDATEUSERDIALOG_H

#include "BaseDialog.h"
#include <QComboBox>
#include <QVariantMap>
#include "qtmaterialflatbutton.h"
#include "qtmaterialtextfield.h"

/**
 * @class UpdateUserDialog
 * @brief The UpdateUserDialog class provides a dialog for updating user information with input validation.
 * @details This class sets up the user interface, connects signals, and handles user input for updating user information.
 */
class UpdateUserDialog : public BaseDialog
{
	Q_OBJECT

public:
	/**
	 * @brief Constructor for UpdateUserDialog.
	 * @param data The initial data for the user.
	 * @param parent The parent widget.
	 */
	UpdateUserDialog(QVariantMap data, QWidget* parent = nullptr);

	/**
	 * @brief Retrieves the data entered in the dialog.
	 * @return A QVariantMap containing the updated user data.
	 */
	QVariantMap getData() const override;

private slots:
	/**
	 * @brief Slot to handle changes in the role selection.
	 */
	void onRoleChanged();

private:
	/**
	 * @brief Sets up the user interface.
	 */
	void setupUI() override;

	/**
	 * @brief Connects signals for the dialog.
	 */
	void connectSignals() override;

	QString		role_;					///< The initial role of the user.
	QVariantMap data_;					///< The initial data for the user.

	QtMaterialTextField* firstNameEdit; ///< Input field for the first name.
	QtMaterialTextField* lastNameEdit;	///< Input field for the last name.
	QtMaterialTextField* emailEdit;		///< Input field for the email.
	QComboBox*			 roleComboBox;	///< Combo box for selecting the user role.

	QLabel* notificationErrorLabel;		///< Label for displaying role change notifications.
};

#endif									// UPDATEUSERDIALOG_H