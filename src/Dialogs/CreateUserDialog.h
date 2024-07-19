#ifndef CREATEUSERDIALOG_H
#define CREATEUSERDIALOG_H

#include "BaseDialog.h"
#include "ValidationStrategy.h"
#include <QComboBox>

class CreateUserDialog : public BaseDialog
{
	Q_OBJECT

public:
	CreateUserDialog(QWidget* parent = nullptr);

	QVariantMap getData() const override;

private slots:
	void onRoleChanged();

private:
	void setupUI() override;
	void connectSignals() override;

	QtMaterialTextField* firstNameEdit;
	QtMaterialTextField* lastNameEdit;
	QtMaterialTextField* emailEdit;
	QtMaterialTextField* passwordEdit;
	QtMaterialTextField* confirmPasswordEdit;
	QtMaterialTextField* initialBalanceEdit;

	QComboBox* roleComboBox;
};

#endif // CREATEUSERDIALOG_H
