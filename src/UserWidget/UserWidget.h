#ifndef USERWIDGET_H
#define USERWIDGET_H

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

#include "RequestManager.h"

class UserWidget : public QWidget
{
	Q_OBJECT

public:
	UserWidget(QString email, QString first_name, QString account_number, QString balance, QWidget* parent = nullptr);

signals:
	void logout();

public slots:
	void onTransactionsFetched(const QList<QMap<QString, QString>>& transactions);
	void onBalanceFetched(const QString balance);
	void onSuccessfullRequest(QString message);
	void onFailedRequest(QString message);

private slots:
	void onUpdateEmail();
	void onUpdatePassword();
	void onLogoutClicked();
	void onLogoutConfirmed();
	void updateTransactionsTable();
	void onBalanceLabelClicked();
	void updateFirstNameLabel();
	void onTransferFieldsChanged();
	void onTransferButtonClicked();

private:
	QWidget*	 createHomeTab();
	QWidget*	 createTransferTab();
	QWidget*	 createSettingsTab();
	QVBoxLayout* createTabLayout();

	QString						  email_;
	QString						  first_name_;
	QString						  account_number_;
	QString						  balance_;
	QList<QMap<QString, QString>> transactions_;
	RequestManager*				  requestManager;

	QtMaterialSnackbar*	  notificationSnackbar;
	QtMaterialTabs*		  tabs;
	QStackedWidget*		  tabContents;
	QtMaterialDialog*	  logoutDialog;
	QtMaterialFlatButton* balanceLabel;
	QtMaterialFlatButton* welcomeLabel;

	QtMaterialTextField*  toAccountField;
	QtMaterialTextField*  toEmailField;
	QtMaterialTextField*  amountField;
	QtMaterialFlatButton* transferButton;

	QTableWidget* transactionsTable;
};

#endif // USERWIDGET_H
