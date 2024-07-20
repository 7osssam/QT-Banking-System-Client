/**
 * @file UpdateEmailDialog.h
 * @brief Header file for the UpdateEmailDialog class.
 * @details Declares the UpdateEmailDialog class which provides a dialog for updating user email with input validation.
**/

#ifndef UPDATEEMAILDIALOG_H
#define UPDATEEMAILDIALOG_H

#include "BaseDialog.h"

/**
 * @class UpdateEmailDialog
 * @brief The UpdateEmailDialog class provides a dialog for updating user email with input validation.
 * @details This class sets up the user interface and retrieves the data entered by the user.
 */
class UpdateEmailDialog : public BaseDialog
{
	Q_OBJECT

public:
	/**
	 * @brief Constructor for UpdateEmailDialog.
	 * @param parent The parent widget.
	 */
	UpdateEmailDialog(QWidget* parent = nullptr);

	/**
	 * @brief Retrieves the data entered in the dialog.
	 * @return A QVariantMap containing the email update data.
	 */
	QVariantMap getData() const override;

protected:
	/**
	 * @brief Sets up the user interface.
	 */
	void setupUI() override;

private:
	QtMaterialTextField* emailField;	///< Input field for the current email.
	QtMaterialTextField* newEmailField; ///< Input field for the new email.
	QtMaterialTextField* passwordField; ///< Input field for the password.
};

#endif									// UPDATEEMAILDIALOG_H
