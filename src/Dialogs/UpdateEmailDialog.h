// UpdateEmailDialog.h
#ifndef UPDATEEMAILDIALOG_H
#define UPDATEEMAILDIALOG_H

#include "BaseDialog.h"

class UpdateEmailDialog : public BaseDialog
{
	Q_OBJECT

public:
	UpdateEmailDialog(QWidget* parent = nullptr);
	QVariantMap getData() const override;

protected:
	void setupUI() override;

private:
	QtMaterialTextField* emailField;
	QtMaterialTextField* newEmailField;
	QtMaterialTextField* passwordField;
};

#endif // UPDATEEMAILDIALOG_H
