#ifndef UPDATEEMAILDIALOG_H
#define UPDATEEMAILDIALOG_H

#include <QDialog>
#include <QLabel>
#include "qtmaterialtextfield.h"
#include "qtmaterialflatbutton.h"

class UpdateEmailDialog : public QDialog
{
	Q_OBJECT

public:
	explicit UpdateEmailDialog(QWidget* parent = nullptr);

	QString getEmail() const;
	QString getNewEmail() const;
	QString getPassword() const;

private slots:
	void onEmailFieldChanged();
	void onNewEmailFieldChanged();
	void onPasswordFieldChanged();

private:
	QtMaterialTextField* emailField;
	QLabel*				 emailErrorLabel;
	QtMaterialTextField* newEmailField;
	QLabel*				 newEmailErrorLabel;
	QtMaterialTextField* passwordField;
	QLabel*				 passwordErrorLabel;

	QtMaterialFlatButton* confirmButton;
	QtMaterialFlatButton* cancelButton;

	bool isValidEmail(const QString& email);
	void setFieldValidityStyle(QtMaterialTextField* field, QLabel* errorLabel, const QString& errorMessage,
							   bool isValid);
	void updateConfirmButtonState();
};

#endif // UPDATEEMAILDIALOG_H
