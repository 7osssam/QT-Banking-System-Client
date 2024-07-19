#include "UpdateUserDialog.h"
#include <QMessageBox>
#include <QBoxLayout>

UpdateUserDialog::UpdateUserDialog(QVariantMap data, QWidget* parent) : BaseDialog(parent), data_(data)
{
	setupUI();

	firstNameEdit->setText(data["first_name"].toString());
	lastNameEdit->setText(data["last_name"].toString());
	emailEdit->setText(data["email"].toString());
	roleComboBox->setCurrentText(data["role"].toString());

	role_ = data["role"].toString();

	connectSignals();
}

QVariantMap UpdateUserDialog::getData() const
{
	QVariantMap userData;
	userData["account_number"] = data_["account_number"];
	userData["first_name"] = firstNameEdit->text();
	userData["last_name"] = lastNameEdit->text();
	userData["email"] = emailEdit->text();
	userData["role"] = roleComboBox->currentText();
	userData["balance"] = data_["balance"];

	return userData;
}

void UpdateUserDialog::setupUI()
{
	setWindowTitle("Update User");
	setModal(true);
	resize(400, 300);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);

	firstNameEdit = createInputField(mainLayout, "First Name:", new NameValidationStrategy);
	lastNameEdit = createInputField(mainLayout, "Last Name:", new NameValidationStrategy);
	emailEdit = createInputField(mainLayout, "Email:", new EmailValidationStrategy);

	QLabel* roleLabel = new QLabel("Role:", this);
	roleComboBox = new QComboBox(this);
	roleComboBox->addItem("user");
	roleComboBox->addItem("admin");
	mainLayout->addWidget(roleLabel);
	mainLayout->addWidget(roleComboBox);

	QHBoxLayout* buttonsLayout = new QHBoxLayout();
	confirmButton = new QtMaterialFlatButton("Update", Material::ButtonTextPrimary, this);
	confirmButton->setRole(Material::Primary);
	confirmButton->setEnabled(true);

	cancelButton = new QtMaterialFlatButton("Cancel", Material::ButtonTextSecondary, this);
	cancelButton->setRole(Material::Secondary);

	buttonsLayout->addWidget(confirmButton);
	buttonsLayout->addWidget(cancelButton);
	mainLayout->addLayout(buttonsLayout);

	notificationErrorLabel = new QLabel(this);
	notificationErrorLabel->setStyleSheet("QLabel { color: yellow; }");
	notificationErrorLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	notificationErrorLabel->setVisible(false);

	mainLayout->addWidget(notificationErrorLabel);
}

void UpdateUserDialog::connectSignals()
{
	BaseDialog::connectSignals();
	connect(roleComboBox, &QComboBox::currentTextChanged, this, &UpdateUserDialog::onRoleChanged);
}

void UpdateUserDialog::onRoleChanged()
{
	QString selectedRole = roleComboBox->currentText();
	if (selectedRole != role_)
	{
		// will not happen as admins cannot change roles of other admins
		if (selectedRole == "user")
		{
			notificationErrorLabel->setText("Changing role to 'user' will remove admin privileges from this user.");
		}
		else if (selectedRole == "admin")
		{
			notificationErrorLabel->setText(
				"Changing role to 'admin' will delete this user's bank account and give them admin privileges.");
		}
		notificationErrorLabel->setVisible(true);
	}
	else
	{
		notificationErrorLabel->setVisible(false);
	}
}
