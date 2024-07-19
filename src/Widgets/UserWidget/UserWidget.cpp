#include "userwidget.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QIntValidator>
#include <QRegularExpressionValidator>
#include <QHBoxLayout>
#include <QVariantMap>

#include "UpdateEmailDialog.h"
#include "UpdatePasswordDialog.h"

#include "ValidationStrategy.h"

UserWidget::UserWidget(QString email, QString first_name, QString account_number, QString balance, QWidget* parent) :
	QWidget(parent), email_(email), first_name_(first_name), account_number_(account_number), balance_(balance),
	requestFactory(RequestFactory::getInstance(this))
{
	// set object name
	setObjectName("UserWidget");

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0); // Remove margins
	mainLayout->setSpacing(0);					// Remove spacing

	tabs = new QtMaterialTabs(this);
	mainLayout->addWidget(tabs);

	tabs->addTab("Home");
	tabs->addTab("Transfer");
	tabs->addTab("Settings");

	tabs->setHaloVisible(true);

	tabs->setTabIcon(0, QtMaterialTheme::icon("action", "home"));
	tabs->setTabIcon(1, QtMaterialTheme::icon("communication", "import_export"));
	tabs->setTabIcon(2, QtMaterialTheme::icon("action", "settings"));
	tabs->setIconSize(QSize(24, 24));

	QWidget* homeTab = createHomeTab();
	QWidget* transferTab = createTransferTab();
	QWidget* settingsTab = createSettingsTab();

	tabContents = new QStackedWidget(this);
	tabContents->addWidget(homeTab);
	tabContents->addWidget(transferTab);
	tabContents->addWidget(settingsTab);
	mainLayout->addWidget(tabContents);

	// Login Snackbar
	notificationSnackbar = new QtMaterialSnackbar(this);
	notificationSnackbar->setAutoHideDuration(3000);   // Auto-hide after 3 seconds
	notificationSnackbar->setClickToDismissMode(true); // Allow click to dismiss

	connect(tabs, &QtMaterialTabs::currentChanged, tabContents, &QStackedWidget::setCurrentIndex);

	connect(tabs, &QtMaterialTabs::currentChanged, this, &UserWidget::updateTransactionsTable);

	connect(balanceLabel, &QtMaterialFlatButton::clicked, this, &UserWidget::onBalanceLabelClicked);

	connect(transferButton, &QtMaterialFlatButton::clicked, this, &UserWidget::onTransferButtonClicked);

	connect(toAccountField, &QtMaterialTextField::textChanged, this, &UserWidget::onTransferFieldsChanged);
	connect(toEmailField, &QtMaterialTextField::textChanged, this, &UserWidget::onTransferFieldsChanged);
	connect(amountField, &QtMaterialTextField::textChanged, this, &UserWidget::onTransferFieldsChanged);

	onBalanceFetched(balance_);
}

QWidget* UserWidget::createHomeTab()
{
	QWidget*	 homeTab = new QWidget(this);
	QVBoxLayout* layout = createTabLayout();
	homeTab->setLayout(layout);

	welcomeLabel =
		new QtMaterialFlatButton("Hello " + first_name_ + ", " + account_number_, Material::ButtonTextPrimary, homeTab);
	layout->addWidget(welcomeLabel);

	transactionsTable = new QTableWidget(homeTab);
	transactionsTable->setColumnCount(4);
	transactionsTable->setAccessibleName("Transactions Table");
	transactionsTable->setGridStyle(Qt::NoPen);
	transactionsTable->setHorizontalHeaderLabels({"From Account", "To Account", "Amount", "Date of Transaction"});

	// Set column width ratio
	transactionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	layout->addWidget(transactionsTable);

	// Add flat (unclickable) buttons for user current balance
	balanceLabel = new QtMaterialFlatButton("Current Balance: $" + balance_, Material::ButtonRaisedDefault, homeTab);
	balanceLabel->setForegroundColor(QColor("#EEEEEE"));
	balanceLabel->setBackgroundColor(QColor("#222831"));
	balanceLabel->setRole(Material::Primary);
	balanceLabel->setOverlayStyle(Material::TintedOverlay);
	layout->addWidget(balanceLabel);

	return homeTab;
}

QWidget* UserWidget::createTransferTab()
{
	// Create the input fields
	toAccountField = new QtMaterialTextField(this);
	toAccountField->setPlaceholderText("To Account");
	toAccountField->setEchoMode(QLineEdit::Normal);
	toAccountField->setValidator(new QIntValidator(this));

	toEmailField = new QtMaterialTextField(this);
	toEmailField->setPlaceholderText("To Email");
	toEmailField->setEchoMode(QLineEdit::Normal);
	const QRegularExpression emailPattern(R"((^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,4}$))");
	toEmailField->setValidator(new QRegularExpressionValidator(emailPattern, this));

	amountField = new QtMaterialTextField(this);
	amountField->setLabelText("Amount");
	amountField->setEchoMode(QLineEdit::Normal);
	amountField->setValidator(new QIntValidator(0, 999999999, this));

	transferButton = new QtMaterialFlatButton("Transfer", Material::ButtonTextSecondary, this);

	transferButton->setDisabled(true); // Initially disabled until all fields are filled

	// Create a horizontal layout for toAccountField, "OR" label, and toEmailField
	QHBoxLayout* toAccountEmailLayout = new QHBoxLayout();
	toAccountEmailLayout->addWidget(toAccountField);

	QLabel* orLabel = new QLabel("OR", this);

	orLabel->setAlignment(Qt::AlignCenter); // Center align the "OR" label
	toAccountEmailLayout->addWidget(orLabel);

	toAccountEmailLayout->addWidget(toEmailField);

	// Create the main layout and add widgets
	QVBoxLayout* layout = createTabLayout();
	layout->addSpacing(50);
	layout->addLayout(toAccountEmailLayout);
	layout->addSpacing(20);
	layout->addWidget(amountField);
	layout->addStretch(1);
	layout->addWidget(transferButton);

	QWidget* transferTab = new QWidget(this);
	transferTab->setLayout(layout);

	return transferTab;
}

QWidget* UserWidget::createSettingsTab()
{
	QWidget*	 settingsTab = new QWidget(this);
	QVBoxLayout* layout = createTabLayout();
	settingsTab->setLayout(layout);

	QtMaterialFlatButton* updateEmailButton =
		new QtMaterialFlatButton("Update Email", Material::ButtonTextDefault, settingsTab);

	updateEmailButton->setForegroundColor(QColor("#EEEEEE"));
	updateEmailButton->setBackgroundColor(QColor("#222831"));
	updateEmailButton->setRole(Material::Primary);
	updateEmailButton->setOverlayStyle(Material::TintedOverlay);
	connect(updateEmailButton, &QPushButton::clicked, this, &UserWidget::onUpdateEmail);
	layout->addStretch(1);

	layout->addWidget(updateEmailButton);

	QtMaterialFlatButton* updatePasswordButton =
		new QtMaterialFlatButton("Update Password", Material::ButtonTextDefault, settingsTab);

	updatePasswordButton->setForegroundColor(QColor("#EEEEEE"));
	updatePasswordButton->setBackgroundColor(QColor("#222831"));
	updatePasswordButton->setRole(Material::Primary);
	updatePasswordButton->setOverlayStyle(Material::TintedOverlay);
	connect(updatePasswordButton, &QPushButton::clicked, this, &UserWidget::onUpdatePassword);
	layout->addStretch(1);
	layout->addWidget(updatePasswordButton);

	QtMaterialFlatButton* logoutButton = new QtMaterialFlatButton("Logout", Material::ButtonTextDefault, settingsTab);

	logoutButton->setForegroundColor(QColor("#EEEEEE"));
	logoutButton->setBackgroundColor(QColor("#FF0000")); // this should be light red
	logoutButton->setUseThemeColors(true);

	logoutButton->setRole(Material::Primary);
	logoutButton->setOverlayStyle(Material::TintedOverlay);

	logoutButton->setIcon(QtMaterialTheme::icon("action", "exit_to_app"));
	logoutButton->setCornerRadius(20);

	connect(logoutButton, &QPushButton::clicked, this, &UserWidget::onLogoutClicked);
	layout->addStretch(4);
	layout->addWidget(logoutButton);

	// Initialize the logout confirmation dialog
	logoutDialog = new QtMaterialDialog(this);
	QWidget*	 dialogWidget = new QWidget();
	QVBoxLayout* dialogWidgetLayout = new QVBoxLayout(dialogWidget);

	QLabel* dialogLabel = new QLabel("Are you sure you want to logout?", dialogWidget);

	dialogLabel->setAlignment(Qt::AlignCenter);
	dialogLabel->setStyleSheet("font-size: 16px; color: #222831;");

	dialogWidgetLayout->addWidget(dialogLabel);

	QHBoxLayout* buttonsLayout = new QHBoxLayout;

	QtMaterialFlatButton* confirmButton = new QtMaterialFlatButton("Yes", Material::ButtonRaisedPrimary, dialogWidget);
	connect(confirmButton, &QPushButton::clicked, this, &UserWidget::onLogoutConfirmed, Qt::QueuedConnection);
	buttonsLayout->addWidget(confirmButton);

	QtMaterialFlatButton* cancelButton =
		new QtMaterialFlatButton("Cancel", Material::ButtonOutlinedSecondary, dialogWidget);
	connect(cancelButton, &QPushButton::clicked, logoutDialog, &QtMaterialDialog::hideDialog);
	buttonsLayout->addWidget(cancelButton);

	dialogWidgetLayout->addLayout(buttonsLayout);
	dialogWidget->setLayout(dialogWidgetLayout);

	QVBoxLayout* dialogLayout = new QVBoxLayout;
	dialogWidget->setMinimumHeight(150);
	dialogLayout->addWidget(dialogWidget);
	logoutDialog->setWindowLayout(dialogLayout);

	return settingsTab;
}

QVBoxLayout* UserWidget::createTabLayout()
{
	QVBoxLayout* layout = new QVBoxLayout();
	layout->setAlignment(Qt::AlignTop);
	layout->setContentsMargins(10, 10, 10, 10); // Adjust margins as needed
	layout->setSpacing(10);						// Adjust spacing as needed
	return layout;
}

void UserWidget::onUpdateEmail()
{
	UpdateEmailDialog dialog(this);
	QVariantMap		  data;

	if (dialog.exec() == QDialog::Accepted)
	{
		data = dialog.getData();

		if (data["email"] == data["new_email"])
		{
			onFailedRequest("Old and new email are the same");
			return;
		}
		if (data["email"] != email_)
		{
			onFailedRequest("You didn't enter your email correctly");
			return;
		}

		requestFactory->createRequest(RequestFactory::UpdateEmail, data);
	}
}

void UserWidget::onUpdatePassword()
{
	UpdatePasswordDialog dialog(this);
	QVariantMap			 data;

	if (dialog.exec() == QDialog::Accepted)
	{
		data = dialog.getData();

		// print the email, current password, new password and repeated new password for debugging
		qDebug() << "Email: " << data["email"];
		qDebug() << "Current Password: " << data["password"];
		qDebug() << "New Password: " << data["new_password"];
		qDebug() << "Repeated New Password: " << data["repeated_new_password"];

		if (data["current_password"] == data["new_password"])
		{
			onFailedRequest("Old and new password are the same");
			return;
		}

		if (data["email"] != email_)
		{
			onFailedRequest("You didn't enter your email correctly");
			return;
		}

		// Send request to update password
		requestFactory->createRequest(RequestFactory::UpdatePassword, data);
	}
}

void UserWidget::onLogoutClicked()
{
	// Show the logout confirmation dialog
	logoutDialog->showDialog();
}

void UserWidget::onLogoutConfirmed()
{
	// Logic for logout confirmation
	logoutDialog->hideDialog();

	// Emit the logout signal
	emit logout();
}

void UserWidget::updateTransactionsTable()
{
	// Send the request to get the transaction history
	if (tabContents->currentIndex() == 0)
	{
		requestFactory->createRequest(RequestFactory::GetTransactionsHistory, QVariantMap({{"email", email_}}));
	}
}

void UserWidget::onTransactionsFetched(const QList<QMap<QString, QString>>& transactions)
{
	transactions_ = transactions;

	transactionsTable->setRowCount(0); // Clear existing rows
	transactionsTable->selectedItems().isEmpty();

	for (const auto& transaction: transactions_)
	{
		int row = transactionsTable->rowCount();
		transactionsTable->insertRow(row);

		QTableWidgetItem* fromAccountItem = new QTableWidgetItem(transaction["from_account_number"]);
		QTableWidgetItem* toAccountItem = new QTableWidgetItem(transaction["to_account_number"]);
		QTableWidgetItem* amountItem = new QTableWidgetItem(transaction["transaction_amount"]);
		QTableWidgetItem* dateItem = new QTableWidgetItem(transaction["created_at"]);

		transactionsTable->setItem(row, 0, fromAccountItem);
		transactionsTable->setItem(row, 1, toAccountItem);
		transactionsTable->setItem(row, 2, amountItem);
		transactionsTable->setItem(row, 3, dateItem);

		transactionsTable->setRowHeight(row, 30);

		// make the table read only
		transactionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
		transactionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
		transactionsTable->setShowGrid(false);
		transactionsTable->setAlternatingRowColors(true);
		transactionsTable->horizontalHeader()->setStretchLastSection(true);
		transactionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		transactionsTable->verticalHeader()->setVisible(false);
	}
	onSuccessfullRequest("Transactions updated Successfully");
}

void UserWidget::onBalanceLabelClicked()
{
	// Send the request to get the balance
	QVariantMap data;
	data["account_number"] = account_number_;
	requestFactory->createRequest(RequestFactory::GetBalance, data);
}

void UserWidget::onBalanceFetched(const QString balance)
{
	balance_ = balance;
	balanceLabel->setText("Current Balance: $" + balance_);

	onSuccessfullRequest("Balance updated Successfully");
}

void UserWidget::updateFirstNameLabel()
{
	welcomeLabel->setText(QString("Hello %1, %2").arg(first_name_).arg(account_number_));
}
void UserWidget::onTransferFieldsChanged()
{
	// Create validation strategy instances
	AccountNumberValidationStrategy accountNumberValidationStrategy;
	EmailValidationStrategy			emailValidationStrategy;
	BalanceValidationStrategy		balanceValidationStrategy;

	// Get field texts
	QString accountText = toAccountField->text();
	QString emailText = toEmailField->text();
	QString amountText = amountField->text();

	// Show or hide the label for the amountField based on whether it's empty
	amountField->setLabelVisible(amountText.isEmpty());

	// Determine if the transfer button should be enabled
	bool isInputValid = (!accountText.isEmpty() || !emailText.isEmpty()) && !amountText.isEmpty();

	// Disable fields based on the presence of text in toAccountField and toEmailField
	toAccountField->setDisabled(!emailText.isEmpty());
	toEmailField->setDisabled(!accountText.isEmpty());

	// Validate and set input line color
	auto validateAndSetColor = [](QtMaterialTextField* field, const IValidationStrategy& strategy) {
		if (strategy.isValid(field->text()) && !field->text().isEmpty())
		{
			field->setInputLineColor(Qt::green);
		}
		else
		{
			field->setInputLineColor(Qt::red);
		}
	};

	validateAndSetColor(toAccountField, accountNumberValidationStrategy);
	validateAndSetColor(toEmailField, emailValidationStrategy);
	validateAndSetColor(amountField, balanceValidationStrategy);

	// Enable transfer button if at least one of the account or email field is valid and amount is valid
	transferButton->setDisabled(
		!isInputValid ||
		(toAccountField->inputLineColor() != Qt::green && toEmailField->inputLineColor() != Qt::green) ||
		amountField->inputLineColor() != Qt::green);
}

void UserWidget::onTransferButtonClicked()
{
	QString toAccount = toAccountField->text();
	QString toEmail = toEmailField->text();
	double	amount = amountField->text().toDouble();

	QVariantMap data;
	data["from_account_number"] = account_number_.toInt();

	data["transaction_amount"] = amount;

	// send request to transfer money wiether using email or account number
	if (!toAccount.isEmpty())
	{
		data["to_email"] = "";
		data["to_account_number"] = toAccount.toInt();
		requestFactory->createRequest(RequestFactory::MakeTransaction, data);
	}
	else if (!toEmail.isEmpty())
	{
		data["to_email"] = toEmail;
		data["to_account_number"] = -1;
		requestFactory->createRequest(RequestFactory::MakeTransaction, data);
	}
}

void UserWidget::onSuccessfullRequest(QString message)
{
	// show snackbar message
	notificationSnackbar->setBackgroundColor(QColor(25, 255, 25));

	notificationSnackbar->addMessage(message);
}

void UserWidget::onFailedRequest(QString message)
{
	// show snackbar message
	notificationSnackbar->setBackgroundColor(QColor(255, 25, 25));

	notificationSnackbar->addMessage(message);
}