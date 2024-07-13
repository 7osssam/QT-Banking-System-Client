#include "updateemaildialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRegularExpressionValidator>

UpdateEmailDialog::UpdateEmailDialog(QWidget* parent) :
	QDialog(parent), emailField(new QtMaterialTextField(this)), emailErrorLabel(new QLabel(this)),
	newEmailField(new QtMaterialTextField(this)), newEmailErrorLabel(new QLabel(this)),
	passwordField(new QtMaterialTextField(this)), passwordErrorLabel(new QLabel(this)),
	confirmButton(new QtMaterialFlatButton("Confirm", Material::ButtonFlatPrimary, this)),
	cancelButton(new QtMaterialFlatButton("Cancel", Material::ButtonFlatSecondary, this))
{
	setWindowTitle("Update Email");

	// set window properties
	setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
	setModal(true);
	setFixedSize(500, 250);

	confirmButton->setEnabled(false); // disable confirm button until all fields are filled

	emailField->setLabelText("Email");
	emailField->setLabelColor(Qt::darkBlue);
	newEmailField->setLabelText("New Email");
	newEmailField->setLabelColor(Qt::darkBlue);
	passwordField->setLabelText("Password");
	passwordField->setLabelColor(Qt::darkBlue);

	emailErrorLabel->setStyleSheet("QLabel { color: red; }");
	emailErrorLabel->setVisible(false);
	emailErrorLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	newEmailErrorLabel->setStyleSheet("QLabel { color: red; }");
	newEmailErrorLabel->setVisible(false);
	newEmailErrorLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	passwordErrorLabel->setStyleSheet("QLabel { color: red; }");
	passwordErrorLabel->setVisible(false);
	passwordErrorLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(emailField);
	layout->addWidget(emailErrorLabel);
	layout->addWidget(newEmailField);
	layout->addWidget(newEmailErrorLabel);
	layout->addWidget(passwordField);
	layout->addWidget(passwordErrorLabel);

	QHBoxLayout* buttonLayout = new QHBoxLayout();
	buttonLayout->addWidget(confirmButton);
	buttonLayout->addWidget(cancelButton);

	layout->addLayout(buttonLayout);

	connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
	connect(confirmButton, &QPushButton::clicked, this, &QDialog::accept);

	connect(emailField, &QtMaterialTextField::textChanged, this, &UpdateEmailDialog::onEmailFieldChanged);
	connect(newEmailField, &QtMaterialTextField::textChanged, this, &UpdateEmailDialog::onNewEmailFieldChanged);
	connect(passwordField, &QtMaterialTextField::textChanged, this, &UpdateEmailDialog::onPasswordFieldChanged);
}

QString UpdateEmailDialog::getEmail() const
{
	return emailField->text();
}

QString UpdateEmailDialog::getNewEmail() const
{
	return newEmailField->text();
}

QString UpdateEmailDialog::getPassword() const
{
	return passwordField->text();
}

bool UpdateEmailDialog::isValidEmail(const QString& email)
{
	const QRegularExpression emailPattern(R"((^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,4}$))");
	return emailPattern.match(email).hasMatch();
}

void UpdateEmailDialog::setFieldValidityStyle(QtMaterialTextField* field, QLabel* errorLabel,
											  const QString& errorMessage, bool isValid)
{
	if (isValid)
	{
		field->setInputLineColor(Qt::green);
		errorLabel->setVisible(false);
	}
	else
	{
		field->setInputLineColor(Qt::red);
		errorLabel->setText(errorMessage);
		errorLabel->setVisible(true);
	}
}

void UpdateEmailDialog::onEmailFieldChanged()
{
	bool isEmailValid = isValidEmail(emailField->text());
	setFieldValidityStyle(emailField, emailErrorLabel, "Invalid email format", isEmailValid);
	emailField->setLabelVisible(emailField->text().isEmpty());
	updateConfirmButtonState();
}

void UpdateEmailDialog::onNewEmailFieldChanged()
{
	bool isNewEmailValid = isValidEmail(newEmailField->text());
	setFieldValidityStyle(newEmailField, newEmailErrorLabel, "Invalid email format", isNewEmailValid);
	newEmailField->setLabelVisible(newEmailField->text().isEmpty());
	updateConfirmButtonState();
}

void UpdateEmailDialog::onPasswordFieldChanged()
{
	bool isPasswordValid = !passwordField->text().isEmpty();
	setFieldValidityStyle(passwordField, passwordErrorLabel, "Password cannot be empty", isPasswordValid);
	passwordField->setLabelVisible(passwordField->text().isEmpty());
	updateConfirmButtonState();
}

void UpdateEmailDialog::updateConfirmButtonState()
{
	bool isEmailValid = isValidEmail(emailField->text());
	bool isNewEmailValid = isValidEmail(newEmailField->text());
	bool isPasswordValid = !passwordField->text().isEmpty();

	confirmButton->setEnabled(isEmailValid && isNewEmailValid && isPasswordValid);
}
