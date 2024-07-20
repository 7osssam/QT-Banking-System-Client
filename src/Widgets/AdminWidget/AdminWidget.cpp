#include "AdminWidget.h"
#include <QHeaderView>
#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QMessageBox>
#include <QVariantMap>

#include "UpdateUserDialog.h"
#include "CreateUserDialog.h"

#include "UpdateEmailDialog.h"
#include "UpdatePasswordDialog.h"

AdminWidget::AdminWidget(QString email, QString first_name, QWidget* parent) :
	QWidget(parent), admin_email_{email}, admin_first_name_{first_name}, notificationSnackbar{nullptr}, tabs{nullptr},
	tabContents{nullptr}, databaseTable{nullptr}, transactionsTable{nullptr}, updateUserFab{nullptr},
	deleteUserFab{nullptr}, createNewUserFab{nullptr}, selectedUserData{}, welcomeLabel{nullptr}, logoutDialog{nullptr},
	requestManager{RequestManager::getInstance()}
{
	// set object name
	setObjectName("AdminWidget");

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0); // Remove margins
	mainLayout->setSpacing(0);					// Remove spacing

	tabs = new QtMaterialTabs(this);
	mainLayout->addWidget(tabs);

	tabs->addTab("Database");
	tabs->addTab("Transactions");
	tabs->addTab("Settings");
	tabs->setBackgroundColor(QColor("#222831"));
	tabs->setInkColor(QColor("#00BCD4"));
	tabs->setHaloVisible(true);

	tabs->setTabIcon(0, QtMaterialTheme::icon("action", "view_list"));
	tabs->setTabIcon(1, QtMaterialTheme::icon("action", "history"));
	tabs->setTabIcon(2, QtMaterialTheme::icon("action", "settings"));
	tabs->setIconSize(QSize(24, 24));

	QWidget* databaseTab = createDatabaseTab();
	QWidget* transactionsTab = createTransactionsTab();
	QWidget* settingsTab = createSettingsTab();

	tabContents = new QStackedWidget(this);
	tabContents->addWidget(databaseTab);
	tabContents->addWidget(transactionsTab);
	tabContents->addWidget(settingsTab);
	mainLayout->addWidget(tabContents);

	// Login Snackbar
	notificationSnackbar = new QtMaterialSnackbar(this);
	notificationSnackbar->setAutoHideDuration(3000);   // Auto-hide after 3 seconds
	notificationSnackbar->setClickToDismissMode(true); // Allow click to dismiss
	notificationSnackbar->setFont(QFont("Fira Sans", 16, QFont::ExtraBold));

	connect(tabs, &QtMaterialTabs::currentChanged, tabContents, &QStackedWidget::setCurrentIndex);

	connect(tabs, &QtMaterialTabs::currentChanged, this, &AdminWidget::updateDatabaseTable);
	connect(tabs, &QtMaterialTabs::currentChanged, this, &AdminWidget::updateTransactionsTable);

	// Floating Action Buttons
	setupFloatingActionButtons();
}

QWidget* AdminWidget::createDatabaseTab()
{
	QWidget*	 databaseTab = new QWidget(this);
	QVBoxLayout* layout = createTabLayout();
	databaseTab->setLayout(layout);

	welcomeLabel = new QtMaterialFlatButton("Welcome, " + admin_first_name_, Material::ButtonTextPrimary, databaseTab);
	layout->addWidget(welcomeLabel);

	databaseTable = new QTableWidget(databaseTab);
	databaseTable->setColumnCount(6);
	databaseTable->setHorizontalHeaderLabels({"Account Number", "First Name", "Last Name", "Email", "Role", "Balance"});
	databaseTable->setGridStyle(Qt::NoPen);
	databaseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	layout->addWidget(databaseTable);

	connect(databaseTable->selectionModel(), &QItemSelectionModel::selectionChanged, this,
			&AdminWidget::onUserSelectionChanged);

	return databaseTab;
}

QWidget* AdminWidget::createTransactionsTab()
{
	QWidget*	 transactionsTab = new QWidget(this);
	QVBoxLayout* layout = createTabLayout();
	transactionsTab->setLayout(layout);

	transactionsTable = new QTableWidget(transactionsTab);
	transactionsTable->setColumnCount(4);
	transactionsTable->setHorizontalHeaderLabels({"From Account", "To Account", "Amount", "Date of Transaction"});
	transactionsTable->setGridStyle(Qt::NoPen);
	transactionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	layout->addWidget(transactionsTable);

	return transactionsTab;
}

QWidget* AdminWidget::createSettingsTab()
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
	updateEmailButton->setFont(QFont("Fira Sans", 20, QFont::ExtraBold));
	connect(updateEmailButton, &QPushButton::clicked, this, &AdminWidget::onUpdateEmail);
	layout->addStretch(1);
	layout->addWidget(updateEmailButton);

	QtMaterialFlatButton* updatePasswordButton =
		new QtMaterialFlatButton("Update Password", Material::ButtonTextDefault, settingsTab);
	updatePasswordButton->setForegroundColor(QColor("#EEEEEE"));
	updatePasswordButton->setBackgroundColor(QColor("#222831"));
	updatePasswordButton->setRole(Material::Primary);
	updatePasswordButton->setOverlayStyle(Material::TintedOverlay);
	updatePasswordButton->setFont(QFont("Fira Sans", 20, QFont::ExtraBold));
	connect(updatePasswordButton, &QPushButton::clicked, this, &AdminWidget::onUpdatePassword);
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
	logoutButton->setFont(QFont("Fira Sans", 20, QFont::Light));
	connect(logoutButton, &QPushButton::clicked, this, &AdminWidget::onLogoutClicked);
	layout->addStretch(4);
	layout->addWidget(logoutButton);

	// Initialize the logout confirmation dialog
	logoutDialog = new QtMaterialDialog(this);
	QWidget*	 dialogWidget = new QWidget();
	QVBoxLayout* dialogWidgetLayout = new QVBoxLayout(dialogWidget);

	QLabel* dialogLabel = new QLabel("Are you sure you want to logout?", dialogWidget);
	dialogLabel->setFont(QFont("Fira Sans", 20, QFont::Light));
	dialogLabel->setAlignment(Qt::AlignCenter);
	dialogLabel->setStyleSheet("font-size: 16px; color: #222831;");
	dialogWidgetLayout->addWidget(dialogLabel);

	QHBoxLayout* buttonsLayout = new QHBoxLayout;

	QtMaterialFlatButton* confirmButton = new QtMaterialFlatButton("Yes", Material::ButtonRaisedPrimary, dialogWidget);
	connect(confirmButton, &QPushButton::clicked, this, &AdminWidget::onLogoutConfirmed, Qt::QueuedConnection);
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

QVBoxLayout* AdminWidget::createTabLayout()
{
	QVBoxLayout* layout = new QVBoxLayout();
	layout->setAlignment(Qt::AlignTop);
	layout->setContentsMargins(10, 10, 10, 10); // Adjust margins as needed
	layout->setSpacing(10);						// Adjust spacing as needed
	return layout;
}

void AdminWidget::onUpdateEmail()
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
		if (data["email"] != admin_email_)
		{
			onFailedRequest("You didn't enter your email correctly");
			return;
		}

		requestManager->createRequest(RequestManager::UpdateEmail, data);
	}
}

void AdminWidget::onUpdatePassword()
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

		if (data["email"] != admin_email_)
		{
			onFailedRequest("You didn't enter your email correctly");
			return;
		}

		// Send request to update password
		requestManager->createRequest(RequestManager::UpdatePassword, data);
	}
}

void AdminWidget::onLogoutClicked()
{
	// Show the logout confirmation dialog
	logoutDialog->showDialog();
}

void AdminWidget::onLogoutConfirmed()
{
	// Logic for logout confirmation
	logoutDialog->hideDialog();

	// Emit the logout signal
	emit logout();
}

void AdminWidget::onDatabaseContentUpdated(const QList<QMap<QString, QString>>& data)
{
	databaseContent_ = data;

	databaseTable->setRowCount(0); // Clear existing rows
	databaseTable->selectedItems().isEmpty();

	for (const auto& user: databaseContent_)
	{
		int row = databaseTable->rowCount();
		databaseTable->insertRow(row);

		QTableWidgetItem* accountNumberItem = new QTableWidgetItem(user.value("account_number"));
		QTableWidgetItem* firstNameItem = new QTableWidgetItem(user.value("first_name"));
		QTableWidgetItem* lastNameItem = new QTableWidgetItem(user.value("last_name"));
		QTableWidgetItem* emailItem = new QTableWidgetItem(user.value("email"));
		QTableWidgetItem* roleItem = new QTableWidgetItem(user.value("role"));
		QTableWidgetItem* balanceItem = new QTableWidgetItem(user.value("balance"));

		databaseTable->setItem(row, 0, accountNumberItem);
		databaseTable->setItem(row, 1, firstNameItem);
		databaseTable->setItem(row, 2, lastNameItem);
		databaseTable->setItem(row, 3, emailItem);
		databaseTable->setItem(row, 4, roleItem);
		databaseTable->setItem(row, 5, balanceItem);

		databaseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
		databaseTable->setSelectionBehavior(QAbstractItemView::SelectRows);
		databaseTable->setShowGrid(false);
		databaseTable->setAlternatingRowColors(true);
		databaseTable->horizontalHeader()->setStretchLastSection(true);
		databaseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		databaseTable->verticalHeader()->setVisible(false);
	}

	onSuccessfullRequest("Database updated Successfully");
}

void AdminWidget::onTransactionsFetched(const QList<QMap<QString, QString>>& transactions)
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

		databaseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
		databaseTable->setSelectionBehavior(QAbstractItemView::SelectRows);
		databaseTable->setShowGrid(false);
		databaseTable->setAlternatingRowColors(true);
		databaseTable->horizontalHeader()->setStretchLastSection(true);
		databaseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		databaseTable->verticalHeader()->setVisible(false);
	}

	onSuccessfullRequest("Transactions updated Successfully");
}

void AdminWidget::onSuccessfullRequest(QString message)
{
	// show snackbar message
	notificationSnackbar->setBackgroundColor(QColor(0, 200, 0, 255)); // Solid green

	notificationSnackbar->addMessage(message);
}

void AdminWidget::onFailedRequest(QString message)
{
	notificationSnackbar->setBackgroundColor(QColor(200, 0, 0, 255)); // Solid red

	notificationSnackbar->addMessage(message);
}

void AdminWidget::updateDatabaseTable()
{
	if (tabs->currentIndex() == 0)
	{
		QVariantMap data;
		data.insert("email", admin_email_);

		requestManager->createRequest(RequestManager::GetDatabase, data);

		createNewUserFab->show();
		updateUserFab->show();
		deleteUserFab->show();
	}
	else
	{
		createNewUserFab->hide();
		updateUserFab->hide();
		deleteUserFab->hide();
	}
}

void AdminWidget::updateTransactionsTable()
{
	if (tabs->currentIndex() == 1)
	{
		QVariantMap data;
		data.insert("email", admin_email_);

		requestManager->createRequest(RequestManager::GetTransactionsHistory, data);
	}
}

void AdminWidget::setupFloatingActionButtons()
{
	updateUserFab = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("action", "autorenew"), this);
	updateUserFab->setBackgroundColor(QColor("#00BCD4"));
	updateUserFab->setRippleStyle(Material::NoRipple);
	updateUserFab->setDisabled(true); // Initially disabled
	updateUserFab->setCorner(Qt::BottomRightCorner);

	deleteUserFab = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("action", "delete"), this);
	deleteUserFab->setBackgroundColor(QColor("#FF4081"));
	deleteUserFab->setRippleStyle(Material::NoRipple);
	deleteUserFab->setDisabled(true); // Initially disabled
	deleteUserFab->setCorner(Qt::BottomLeftCorner);

	createNewUserFab = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("content", "add"), this);
	createNewUserFab->setBackgroundColor(QColor("#4CAF50"));
	createNewUserFab->setRippleStyle(Material::NoRipple);
	createNewUserFab->setCorner(Qt::BottomRightCorner);

	createNewUserFab->setOffset(100, 37);

	createNewUserFab->setDisabled(false);
	createNewUserFab->show();

	connect(updateUserFab, &QtMaterialFloatingActionButton::clicked, this, &AdminWidget::onUpdateUserClicked);
	connect(deleteUserFab, &QtMaterialFloatingActionButton::clicked, this, &AdminWidget::onDeleteUserClicked);
	connect(createNewUserFab, &QtMaterialFloatingActionButton::clicked, this, &AdminWidget::onCreateNewUserClicked);
}

void AdminWidget::onUpdateUserClicked()
{
	if (selectedUserData.isEmpty())
	{
		return;
	}

	if (selectedUserData.value("role") == "admin")
	{
		onFailedRequest("Cannot update admin user");
		return;
	}

	UpdateUserDialog* dialog = new UpdateUserDialog(selectedUserData, this);

	if (dialog->exec() == QDialog::Accepted)
	{
		QVariantMap newData = dialog->getData();

		QVariantMap data;
		data.insert("email", admin_email_);

		if (selectedUserData.value("role") == "admin")
		{
			newData.remove("role");

			data.insert("account_number", "-1");

			data.insert("newData", newData);

			requestManager->createRequest(RequestManager::UpdateUser, data);
		}
		else if (selectedUserData.value("role") == "user")
		{
			data.insert("account_number", newData.value("account_number").toInt());
			data.insert("newData", newData);

			requestManager->createRequest(RequestManager::UpdateUser, data);
		}
	}
}

void AdminWidget::onCreateNewUserClicked()
{
	CreateUserDialog* dialog = new CreateUserDialog(this);

	if (dialog->exec() == QDialog::Accepted)
	{
		QVariantMap data;
		QVariantMap newUser = dialog->getData();

		data.insert("email", admin_email_);

		data.insert("newUser", newUser);

		// print for debugging userdata
		for (auto i = newUser.begin(); i != newUser.end(); ++i)
		{
			qDebug() << i.key() << ": " << i.value();
		}

		requestManager->createRequest(RequestManager::CreateNewUser, data);
	}
}

void AdminWidget::onDeleteUserClicked()
{
	if (selectedUserData.isEmpty())
	{
		return;
	}

	if (selectedUserData.value("role") == "admin")
	{
		onFailedRequest("Cannot delete admin user");
		return;
	}

	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, "Delete User",
								  "Are you sure you want to delete this user and the associated account\n" +
									  QString::number(selectedUserData.value("account_number").toInt()) + "?",
								  QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes)
	{
		QVariantMap data;
		data.insert("email", admin_email_);
		data.insert("account_number", selectedUserData.value("account_number"));

		requestManager->createRequest(RequestManager::DeleteUser, data);
	}
}

void AdminWidget::onUserSelectionChanged()
{
	QModelIndexList selectedIndexes = databaseTable->selectionModel()->selectedRows();
	if (selectedIndexes.size() == 1)
	{
		selectedUserData.clear();
		int selectedRow = selectedIndexes.first().row();
		for (int i = 0; i < databaseTable->columnCount(); i++)
		{
			// using the same keys as the database table QTableWidget
			selectedUserData.insert("account_number", databaseTable->item(selectedRow, 0)->text().toInt());
			selectedUserData.insert("first_name", databaseTable->item(selectedRow, 1)->text());
			selectedUserData.insert("last_name", databaseTable->item(selectedRow, 2)->text());
			selectedUserData.insert("email", databaseTable->item(selectedRow, 3)->text());
			selectedUserData.insert("role", databaseTable->item(selectedRow, 4)->text());
			selectedUserData.insert("balance", databaseTable->item(selectedRow, 5)->text().toDouble());
		}

		updateUserFab->setDisabled(false);
		deleteUserFab->setDisabled(false);
	}
	else
	{
		selectedUserData.clear();
		updateUserFab->setDisabled(true);
		deleteUserFab->setDisabled(true);
	}
}
