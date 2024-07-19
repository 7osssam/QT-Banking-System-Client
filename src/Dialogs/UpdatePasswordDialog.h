// UpdatePasswordDialog.h
#ifndef UPDATEPASSWORDDIALOG_H
#define UPDATEPASSWORDDIALOG_H

#include "BaseDialog.h"
#include "qtmaterialradiobutton.h"

class UpdatePasswordDialog : public BaseDialog
{
	Q_OBJECT

public:
	UpdatePasswordDialog(QWidget* parent = nullptr);
	QVariantMap getData() const override;

protected:
	void setupUI() override;

private slots:
	void onShowHidePasswordClicked(bool checked);

private:
	QtMaterialTextField*   emailField;
	QtMaterialTextField*   currentPasswordField;
	QtMaterialTextField*   newPasswordField;
	QtMaterialTextField*   repeatedNewPasswordField;
	QtMaterialRadioButton* showHidePasswordRadioButton;
	bool				   isPasswordVisible;
};

#endif // UPDATEPASSWORDDIALOG_H
