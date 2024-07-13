#include "updatepassworddialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRegularExpressionValidator>
#include <QIcon>

UpdatePasswordDialog::UpdatePasswordDialog(QWidget* parent) :
	QDialog(parent), emailField(new QtMaterialTextField(this)), emailErrorLabel(new QLabel(this)),
	currentPasswordField(new QtMaterialTextField(this)), currentPasswordErrorLabel(new QLabel(this)),
	newPasswordField(new QtMaterialTextField(this)), newPasswordErrorLabel(new QLabel(this)),
	repeatedNewPasswordField(new QtMaterialTextField(this)), repeatedNewPasswordErrorLabel(new QLabel(this)),
	confirmButton(new QtMaterialFlatButton("Confirm", Material::ButtonFlatPrimary, this)),
	cancelButton(new QtMaterialFlatButton("Cancel", Material::ButtonFlatSecondary, this)),
	showHidePasswordButton(new QtMaterialIconButton(QtMaterialTheme::icon("toggle", "radio_button_unchecked"), this)),
	isPasswordVisible(false)
{
	setWindowTitle("Update Password");

	// Set window properties
	setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
	setModal(true);
	setFixedSize(600, 300);

	confirmButton->setEnabled(false); // Disable confirm button until all fields are filled
	confirmButton->setIcon(QtMaterialTheme::icon("action", "input"));
	cancelButton->setIcon(QtMaterialTheme::icon("navigation", "close"));

	// Set placeholder text for fields
	emailField->setLabelText("Email");
	emailField->setLabelColor(Qt::darkBlue);

	currentPasswordField->setLabelText("Current Password");
	currentPasswordField->setEchoMode(QLineEdit::Password);
	currentPasswordField->setLabelColor(Qt::darkBlue);

	newPasswordField->setLabelText("New Password");
	newPasswordField->setEchoMode(QLineEdit::Password);
	newPasswordField->setLabelColor(Qt::darkBlue);

	repeatedNewPasswordField->setLabelText("Repeat New Password");
	repeatedNewPasswordField->setEchoMode(QLineEdit::Password);
	repeatedNewPasswordField->setLabelColor(Qt::darkBlue);

	emailErrorLabel->setStyleSheet("QLabel { color: red; }");
	emailErrorLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	emailErrorLabel->setVisible(false);

	currentPasswordErrorLabel->setStyleSheet("QLabel { color: red; }");
	currentPasswordErrorLabel->setVisible(false);
	currentPasswordErrorLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	newPasswordErrorLabel->setStyleSheet("QLabel { color: red; }");
	newPasswordErrorLabel->setVisible(false);
	newPasswordErrorLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	repeatedNewPasswordErrorLabel->setStyleSheet("QLabel { color: red; }");
	repeatedNewPasswordErrorLabel->setVisible(false);
	repeatedNewPasswordErrorLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	// Set icon button properties
	showHidePasswordButton->setIconSize(QSize(24, 24));
	showHidePasswordButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	showHidePasswordButton->setFixedSize(40, 40);
	showHidePasswordButton->setColor(Qt::lightGray);

	// Layout setup
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(emailField);
	layout->addWidget(emailErrorLabel);
	layout->addWidget(currentPasswordField);
	layout->addWidget(currentPasswordErrorLabel);

	// Password fields and show/hide button layout
	QHBoxLayout* newPasswordLayout = new QHBoxLayout();
	newPasswordLayout->addWidget(newPasswordField);
	newPasswordLayout->addWidget(showHidePasswordButton);
	layout->addLayout(newPasswordLayout);
	layout->addWidget(newPasswordErrorLabel); // Add the error label here

	layout->addWidget(repeatedNewPasswordField);
	layout->addWidget(repeatedNewPasswordErrorLabel);

	// Button layout
	QHBoxLayout* buttonLayout = new QHBoxLayout();
	buttonLayout->addWidget(confirmButton);
	buttonLayout->addWidget(cancelButton);
	layout->addLayout(buttonLayout);

	// Connect signals and slots
	connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
	connect(confirmButton, &QPushButton::clicked, this, &QDialog::accept);

	connect(emailField, &QtMaterialTextField::textChanged, this, &UpdatePasswordDialog::onEmailFieldChanged);
	connect(currentPasswordField, &QtMaterialTextField::textChanged, this,
			&UpdatePasswordDialog::onCurrentPasswordFieldChanged);
	connect(newPasswordField, &QtMaterialTextField::textChanged, this,
			&UpdatePasswordDialog::onNewPasswordFieldChanged);
	connect(repeatedNewPasswordField, &QtMaterialTextField::textChanged, this,
			&UpdatePasswordDialog::onRepeatedNewPasswordFieldChanged);
	connect(showHidePasswordButton, &QtMaterialIconButton::clicked, this,
			&UpdatePasswordDialog::onShowHidePasswordClicked);
}

QString UpdatePasswordDialog::getEmail() const
{
	return emailField->text();
}

QString UpdatePasswordDialog::getCurrentPassword() const
{
	return currentPasswordField->text();
}

QString UpdatePasswordDialog::getNewPassword() const
{
	return newPasswordField->text();
}

QString UpdatePasswordDialog::getRepeatedNewPassword() const
{
	return repeatedNewPasswordField->text();
}

bool UpdatePasswordDialog::isValidEmail(const QString& email)
{
	const QRegularExpression emailPattern(R"((^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,4}$))");
	return emailPattern.match(email).hasMatch();
}

bool UpdatePasswordDialog::isValidPassword(const QString& password)
{
	const QRegularExpression passwordPattern(
		R"((?=.*\d)(?=.*[a-z])(?=.*[A-Z]).{8,})"); // at least 8 characters, 1 digit, 1 lowercase, 1 uppercase
	return passwordPattern.match(password).hasMatch();
}

void UpdatePasswordDialog::setFieldValidityStyle(QtMaterialTextField* field, QLabel* errorLabel,
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

void UpdatePasswordDialog::onEmailFieldChanged()
{
	bool isEmailValid = isValidEmail(emailField->text());
	setFieldValidityStyle(emailField, emailErrorLabel, "Invalid email format", isEmailValid);

	emailField->setLabelVisible(emailField->text().isEmpty());
	updateConfirmButtonState();
}

void UpdatePasswordDialog::onCurrentPasswordFieldChanged()
{
	bool isCurrentPasswordValid = !currentPasswordField->text().isEmpty();
	setFieldValidityStyle(currentPasswordField, currentPasswordErrorLabel, "Current password cannot be empty",
						  isCurrentPasswordValid);

	currentPasswordField->setLabelVisible(currentPasswordField->text().isEmpty());

	updateConfirmButtonState();
}

void UpdatePasswordDialog::onNewPasswordFieldChanged()
{
	bool isNewPasswordValid = !newPasswordField->text().isEmpty();
	setFieldValidityStyle(newPasswordField, newPasswordErrorLabel, "New password cannot be empty", isNewPasswordValid);

	isNewPasswordValid = isNewPasswordValid && isValidPassword(newPasswordField->text());

	setFieldValidityStyle(newPasswordField, newPasswordErrorLabel,
						  "Password must contain at least 8 characters, 1 digit, 1 lowercase, 1 uppercase",
						  isNewPasswordValid);

	newPasswordField->setLabelVisible(newPasswordField->text().isEmpty());

	updateConfirmButtonState();
}

void UpdatePasswordDialog::onRepeatedNewPasswordFieldChanged()
{
	bool isRepeatedNewPasswordValid = !repeatedNewPasswordField->text().isEmpty();
	bool isNewPasswordsMatch = newPasswordField->text() == repeatedNewPasswordField->text();
	setFieldValidityStyle(repeatedNewPasswordField, repeatedNewPasswordErrorLabel, "Passwords do not match",
						  isNewPasswordsMatch && isRepeatedNewPasswordValid);

	repeatedNewPasswordField->setLabelVisible(repeatedNewPasswordField->text().isEmpty());

	updateConfirmButtonState();
}

void UpdatePasswordDialog::onShowHidePasswordClicked()
{
	if (isPasswordVisible)
	{
		showHidePasswordButton->setIcon(QtMaterialTheme::icon("toggle", "radio_button_unchecked"));
	}
	else
	{
		showHidePasswordButton->setIcon(QtMaterialTheme::icon("toggle", "radio_button_checked"));
	}

	isPasswordVisible = !isPasswordVisible;
	QLineEdit::EchoMode echoMode = isPasswordVisible ? QLineEdit::Normal : QLineEdit::Password;
	currentPasswordField->setEchoMode(echoMode);
	newPasswordField->setEchoMode(echoMode);
	repeatedNewPasswordField->setEchoMode(echoMode);
}

void UpdatePasswordDialog::updateConfirmButtonState()
{
	bool isEmailValid = isValidEmail(emailField->text());
	bool isCurrentPasswordValid = !currentPasswordField->text().isEmpty();
	bool isNewPasswordValid = !newPasswordField->text().isEmpty();
	bool isRepeatedNewPasswordValid = !repeatedNewPasswordField->text().isEmpty();
	bool isNewPasswordsMatch = newPasswordField->text() == repeatedNewPasswordField->text();

	isNewPasswordValid = isNewPasswordValid && isValidPassword(newPasswordField->text());

	confirmButton->setEnabled(isEmailValid && isCurrentPasswordValid && isNewPasswordValid &&
							  isRepeatedNewPasswordValid && isNewPasswordsMatch);
}
