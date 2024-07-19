
// UpdatePasswordDialog.cpp
#include "UpdatePasswordDialog.h"

UpdatePasswordDialog::UpdatePasswordDialog(QWidget* parent) : BaseDialog(parent), isPasswordVisible(false)
{
	setupUI();
	connectSignals();
}

QVariantMap UpdatePasswordDialog::getData() const
{
	QVariantMap data;
	data.insert("email", emailField->text());
	data.insert("password", currentPasswordField->text());
	data.insert("new_password", newPasswordField->text());
	data.insert("repeated_new_password", repeatedNewPasswordField->text());
	return data;
}

void UpdatePasswordDialog::setupUI()
{
	setWindowTitle("Update Password");
	setModal(true);
	setFixedSize(600, 400);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	QVBoxLayout* passwordLayout = new QVBoxLayout(this);

	emailField = createInputField(mainLayout, "Email:", new EmailValidationStrategy);
	currentPasswordField = createInputField(mainLayout, "Current Password:", new PasswordValidationStrategy, true);
	newPasswordField = createInputField(passwordLayout, "New Password:", new PasswordValidationStrategy, true);
	repeatedNewPasswordField =
		createInputField(passwordLayout, "Repeat New Password:", new PasswordValidationStrategy, true);

	showHidePasswordRadioButton = new QtMaterialRadioButton(this);
	showHidePasswordRadioButton->setChecked(false);
	showHidePasswordRadioButton->setCheckedIcon(QtMaterialTheme::icon("action", "visibility"));
	showHidePasswordRadioButton->setUncheckedIcon(QtMaterialTheme::icon("action", "visibility_off"));
	showHidePasswordRadioButton->setCheckedColor(Qt::white);
	showHidePasswordRadioButton->setUncheckedColor(Qt::gray);

	QHBoxLayout* passwordLayoutWithRadioButton = new QHBoxLayout();
	passwordLayoutWithRadioButton->addLayout(passwordLayout);
	passwordLayoutWithRadioButton->addWidget(showHidePasswordRadioButton);
	mainLayout->addLayout(passwordLayoutWithRadioButton);

	connect(showHidePasswordRadioButton, &QtMaterialRadioButton::clicked, this,
			&UpdatePasswordDialog::onShowHidePasswordClicked);

	passwordLayout->addWidget(showHidePasswordRadioButton);
	mainLayout->addLayout(passwordLayout);

	QHBoxLayout* buttonLayout = new QHBoxLayout();
	confirmButton = new QtMaterialFlatButton("Confirm", Material::ButtonFlatPrimary, this);
	confirmButton->setEnabled(false);
	confirmButton->setIcon(QtMaterialTheme::icon("action", "done"));

	cancelButton = new QtMaterialFlatButton("Cancel", Material::ButtonFlatSecondary, this);
	cancelButton->setIcon(QtMaterialTheme::icon("navigation", "close"));

	buttonLayout->addWidget(confirmButton);
	buttonLayout->addWidget(cancelButton);
	mainLayout->addLayout(buttonLayout);
}

void UpdatePasswordDialog::onShowHidePasswordClicked(bool checked)
{
	isPasswordVisible = checked;
	QLineEdit::EchoMode mode = isPasswordVisible ? QLineEdit::Normal : QLineEdit::Password;

	currentPasswordField->setEchoMode(mode);
	newPasswordField->setEchoMode(mode);
	repeatedNewPasswordField->setEchoMode(mode);
}
