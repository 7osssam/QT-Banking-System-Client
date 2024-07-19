#ifndef UPDATEUSERDIALOG_H
#define UPDATEUSERDIALOG_H

#include "BaseDialog.h"
#include <QComboBox>
#include <QVariantMap>
#include "qtmaterialflatbutton.h"
#include "qtmaterialtextfield.h"

class UpdateUserDialog : public BaseDialog
{
	Q_OBJECT

public:
	UpdateUserDialog(QVariantMap data, QWidget* parent = nullptr);

	QVariantMap getData() const override;

private slots:
	void onRoleChanged();

private:
	void setupUI() override;
	void connectSignals() override;

	QString		role_;
	QVariantMap data_;

	QtMaterialTextField* firstNameEdit;
	QtMaterialTextField* lastNameEdit;
	QtMaterialTextField* emailEdit;
	QComboBox*			 roleComboBox;

	QLabel* notificationErrorLabel;
};

#endif // UPDATEUSERDIALOG_H
