#ifndef UPDATEPASSWORDDIALOG_H
#define UPDATEPASSWORDDIALOG_H

#include <QDialog>
#include <QLabel>
#include "qtmaterialtextfield.h"
#include "qtmaterialflatbutton.h"
#include "qtmaterialiconbutton.h" // Include QtMaterialIconButton

class UpdatePasswordDialog : public QDialog
{
	Q_OBJECT

public:
	explicit UpdatePasswordDialog(QWidget* parent = nullptr);

	QString getEmail() const;
	QString getCurrentPassword() const;
	QString getNewPassword() const;
	QString getRepeatedNewPassword() const;

private slots:
	void onEmailFieldChanged();
	void onCurrentPasswordFieldChanged();
	void onNewPasswordFieldChanged();
	void onRepeatedNewPasswordFieldChanged();
	void onShowHidePasswordClicked();

private:
	QtMaterialTextField* emailField;
	QLabel*				 emailErrorLabel;
	QtMaterialTextField* currentPasswordField;
	QLabel*				 currentPasswordErrorLabel;
	QtMaterialTextField* newPasswordField;
	QLabel*				 newPasswordErrorLabel;
	QtMaterialTextField* repeatedNewPasswordField;
	QLabel*				 repeatedNewPasswordErrorLabel;

	QtMaterialFlatButton* confirmButton;
	QtMaterialFlatButton* cancelButton;
	QtMaterialIconButton* showHidePasswordButton; // Use QtMaterialIconButton for the show/hide button

	bool isPasswordVisible;

	bool isValidEmail(const QString& email);
	bool isValidPassword(const QString& password);
	void setFieldValidityStyle(QtMaterialTextField* field, QLabel* errorLabel, const QString& errorMessage,
							   bool isValid);
	void updateConfirmButtonState();
};

#endif // UPDATEPASSWORDDIALOG_H
