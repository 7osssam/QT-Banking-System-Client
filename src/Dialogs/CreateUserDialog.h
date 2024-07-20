/**
 * @file CreateUserDialog.h
 * @brief Header file for the CreateUserDialog class.
 * @details Declares the CreateUserDialog class which provides a dialog for creating new users with input validation.
**/

#ifndef CREATEUSERDIALOG_H
#define CREATEUSERDIALOG_H

#include "BaseDialog.h"
#include "ValidationStrategy.h"
#include <QComboBox>

/**
 * @class CreateUserDialog
 * @brief The CreateUserDialog class provides a dialog for creating new users with input validation.
 * @details This class sets up the user interface, connects signals, and handles user input for creating new users.
 */
class CreateUserDialog : public BaseDialog
{
	Q_OBJECT

public:
	/**
	 * @brief Constructor for CreateUserDialog.
	 * @param parent The parent widget.
	 */
	CreateUserDialog(QWidget* parent = nullptr);

	/**
	 * @brief Retrieves the data entered in the dialog.
	 * @return A QVariantMap containing the user data.
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

	QtMaterialTextField* firstNameEdit;		  ///< Input field for the first name.
	QtMaterialTextField* lastNameEdit;		  ///< Input field for the last name.
	QtMaterialTextField* emailEdit;			  ///< Input field for the email.
	QtMaterialTextField* passwordEdit;		  ///< Input field for the password.
	QtMaterialTextField* confirmPasswordEdit; ///< Input field for confirming the password.
	QtMaterialTextField* initialBalanceEdit;  ///< Input field for the initial balance.

	QComboBox* roleComboBox;				  ///< Combo box for selecting the user role.
};

#endif										  // CREATEUSERDIALOG_H
