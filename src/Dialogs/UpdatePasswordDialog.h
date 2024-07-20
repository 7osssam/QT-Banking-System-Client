/**
 * @file UpdatePasswordDialog.h
 * @brief Header file for the UpdatePasswordDialog class.
 * @details Declares the UpdatePasswordDialog class which provides a dialog for updating user password with input validation.
**/

#ifndef UPDATEPASSWORDDIALOG_H
#define UPDATEPASSWORDDIALOG_H

#include "BaseDialog.h"
#include "qtmaterialradiobutton.h"

/**
 * @class UpdatePasswordDialog
 * @brief The UpdatePasswordDialog class provides a dialog for updating user passwords with input validation.
 * @details This class sets up the user interface, connects signals, and handles user input for updating passwords.
 */
class UpdatePasswordDialog : public BaseDialog
{
	Q_OBJECT

public:
	/**
	 * @brief Constructor for UpdatePasswordDialog.
	 * @param parent The parent widget.
	 */
	UpdatePasswordDialog(QWidget* parent = nullptr);

	/**
	 * @brief Retrieves the data entered in the dialog.
	 * @return A QVariantMap containing the password update data.
	 */
	QVariantMap getData() const override;

protected:
	/**
	 * @brief Sets up the user interface.
	 */
	void setupUI() override;

private slots:
	/**
	 * @brief Slot to handle the show/hide password toggle.
	 * @param checked Boolean flag indicating the state of the toggle.
	 */
	void onShowHidePasswordClicked(bool checked);

private:
	QtMaterialTextField*   emailField;					///< Input field for the email.
	QtMaterialTextField*   currentPasswordField;		///< Input field for the current password.
	QtMaterialTextField*   newPasswordField;			///< Input field for the new password.
	QtMaterialTextField*   repeatedNewPasswordField;	///< Input field for repeating the new password.
	QtMaterialRadioButton* showHidePasswordRadioButton; ///< Radio button to toggle password visibility.
	bool				   isPasswordVisible;			///< Flag to indicate if the password is visible.
};

#endif													// UPDATEPASSWORDDIALOG_H