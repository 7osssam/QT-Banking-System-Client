// BaseDialog.h
#ifndef BASEDIALOG_H
#define BASEDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVariantMap>
#include "ValidationStrategy.h"
#include "qtmaterialflatbutton.h"
#include "qtmaterialtextfield.h"

class BaseDialog : public QDialog
{
	Q_OBJECT

public:
	BaseDialog(QWidget* parent = nullptr);
	virtual QVariantMap getData() const = 0;

protected:
	virtual void		 setupUI() = 0;
	virtual void		 connectSignals();
	QtMaterialTextField* createInputField(QVBoxLayout* layout, const QString& labelText,
										  IValidationStrategy* validationStrategy, bool isPassword = false);
	void				 validateInput(QtMaterialTextField* field);
	void				 updateConfirmButtonState();
	void setFieldValidityStyle(QtMaterialTextField* field, QLabel* errorLabel, const QString& errorMessage,
							   bool isValid);

	QMap<QtMaterialTextField*, IValidationStrategy*> validationStrategies_;
	QMap<QtMaterialTextField*, QLabel*>				 errorLabels_;
	QtMaterialFlatButton*							 confirmButton;
	QtMaterialFlatButton*							 cancelButton;
};

#endif // BASEDIALOG_H
