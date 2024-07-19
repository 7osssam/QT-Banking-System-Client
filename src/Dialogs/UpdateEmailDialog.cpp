
// UpdateEmailDialog.cpp
#include "UpdateEmailDialog.h"

UpdateEmailDialog::UpdateEmailDialog(QWidget* parent) : BaseDialog(parent)
{
	setupUI();
	connectSignals();
}

QVariantMap UpdateEmailDialog::getData() const
{
	QVariantMap data;
	data.insert("email", emailField->text());
	data.insert("new_email", newEmailField->text());
	data.insert("password", passwordField->text());
	return data;
}

void UpdateEmailDialog::setupUI()
{
	setWindowTitle("Update Email");
	setModal(true);
	setFixedSize(600, 400);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);

	emailField = createInputField(mainLayout, "Current Email:", new EmailValidationStrategy);
	newEmailField = createInputField(mainLayout, "New Email:", new EmailValidationStrategy);
	passwordField = createInputField(mainLayout, "Password:", new PasswordValidationStrategy, true);

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
