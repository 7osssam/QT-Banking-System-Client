#ifndef ADMINWIDGET_H
#define ADMINWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QStackedWidget>
#include <QTableWidget>
#include "qtmaterialtabs.h"
#include "qtmaterialflatbutton.h"
#include "qtmaterialdialog.h"
#include "qtmaterialsnackbar.h"
#include "qtmaterialtextfield.h"
#include "qtmaterialfab.h"
#include "RequestFactory.h"

#include <QVariantMap>

class AdminWidget : public QWidget
{
	Q_OBJECT

public:
	AdminWidget(QString email, QString first_name, QWidget* parent = nullptr);

signals:
	void logout();

public slots:
	void onDatabaseContentUpdated(const QList<QMap<QString, QString>>& data);
	void onTransactionsFetched(const QList<QMap<QString, QString>>& transactions);
	void onSuccessfullRequest(QString message);
	void onFailedRequest(QString message);

private slots:
	void onUpdateEmail();
	void onUpdatePassword();
	void onLogoutClicked();
	void onLogoutConfirmed();
	void updateDatabaseTable();
	void updateTransactionsTable();
	void onUpdateUserClicked();
	void onDeleteUserClicked();
	void onCreateNewUserClicked();
	void onUserSelectionChanged();

private:
	QWidget*	 createDatabaseTab();
	QWidget*	 createTransactionsTab();
	QWidget*	 createSettingsTab();
	QVBoxLayout* createTabLayout();
	void		 setupFloatingActionButtons();

	QString						  admin_email_;
	QString						  admin_first_name_;
	QList<QMap<QString, QString>> transactions_;
	QList<QMap<QString, QString>> databaseContent_;
	RequestFactory*				  requestFactory;

	QtMaterialFlatButton* welcomeLabel;
	QtMaterialSnackbar*	  notificationSnackbar;
	QtMaterialTabs*		  tabs;
	QStackedWidget*		  tabContents;
	QtMaterialDialog*	  logoutDialog;

	QTableWidget* databaseTable;
	QTableWidget* transactionsTable;

	QtMaterialFloatingActionButton* updateUserFab;
	QtMaterialFloatingActionButton* deleteUserFab;
	QtMaterialFloatingActionButton* createNewUserFab;

	QVariantMap selectedUserData;
};

#endif // ADMINWIDGET_H
