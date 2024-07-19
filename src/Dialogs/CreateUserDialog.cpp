
#include "CreateUserDialog.h"
#include <QMessageBox>
#include <QBoxLayout>

CreateUserDialog::CreateUserDialog(QWidget* parent) : BaseDialog(parent)
{
	setupUI();
	connectSignals();
}

QVariantMap CreateUserDialog::getData() const
{
	QVariantMap userData;
	userData["first_name"] = firstNameEdit->text();
	userData["last_name"] = lastNameEdit->text();
	userData["email"] = emailEdit->text();
	userData["password"] = passwordEdit->text();
	userData["role"] = roleComboBox->currentText();
	userData["initial_balance"] = initialBalanceEdit->text().toDouble();

	return userData;
}

void CreateUserDialog::setupUI()
{
	setWindowTitle("Create User");
	setModal(false);
	setFixedSize(500, 600);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);

	firstNameEdit = createInputField(mainLayout, "First Name:", new NameValidationStrategy);
	lastNameEdit = createInputField(mainLayout, "Last Name:", new NameValidationStrategy);
	emailEdit = createInputField(mainLayout, "Email:", new EmailValidationStrategy);
	passwordEdit = createInputField(mainLayout, "Password:", new PasswordValidationStrategy, true);
	confirmPasswordEdit = createInputField(mainLayout, "Confirm Password:", new PasswordValidationStrategy, true);
	initialBalanceEdit = createInputField(mainLayout, "Initial Balance:", new BalanceValidationStrategy);

	QLabel* roleLabel = new QLabel("Role:", this);
	roleComboBox = new QComboBox(this);
	roleComboBox->addItem("user");
	roleComboBox->addItem("admin");
	roleComboBox->setCurrentText("user");
	roleComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	mainLayout->addWidget(roleLabel);
	mainLayout->addWidget(roleComboBox);

	QHBoxLayout* buttonsLayout = new QHBoxLayout();
	confirmButton = new QtMaterialFlatButton("Create", Material::ButtonTextPrimary, this);
	confirmButton->setRole(Material::Primary);
	confirmButton->setEnabled(false);

	cancelButton = new QtMaterialFlatButton("Cancel", Material::ButtonTextSecondary, this);
	cancelButton->setRole(Material::Secondary);

	buttonsLayout->addWidget(confirmButton);
	buttonsLayout->addWidget(cancelButton);
	mainLayout->addLayout(buttonsLayout);
}

void CreateUserDialog::connectSignals()
{
	BaseDialog::connectSignals();

	connect(roleComboBox, &QComboBox::currentTextChanged, this, &CreateUserDialog::onRoleChanged);
}

void CreateUserDialog::onRoleChanged()
{
	QString selectedRole = roleComboBox->currentText();
	if (selectedRole == "admin")
	{
		initialBalanceEdit->setEnabled(false);
		initialBalanceEdit->setText("000");
		initialBalanceEdit->setLabelColor(Qt::gray);
		initialBalanceEdit->setInputLineColor(Qt::gray);
	}
	else
	{
		initialBalanceEdit->setEnabled(true);
		initialBalanceEdit->setText("");
		initialBalanceEdit->setLabelColor(Qt::black);
		initialBalanceEdit->setInputLineColor(Qt::red);
	}
}
