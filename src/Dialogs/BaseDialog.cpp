
// BaseDialog.cpp
#include "BaseDialog.h"

BaseDialog::BaseDialog(QWidget* parent) : QDialog(parent), confirmButton(nullptr), cancelButton(nullptr)
{
}

void BaseDialog::connectSignals()
{
	connect(confirmButton, &QPushButton::clicked, this, &BaseDialog::accept);
	connect(cancelButton, &QPushButton::clicked, this, &BaseDialog::reject);

	for (auto field: validationStrategies_.keys())
	{
		connect(field, &QtMaterialTextField::textChanged, [this, field]() {
			validateInput(field);
		});
	}
}

QtMaterialTextField* BaseDialog::createInputField(QVBoxLayout* layout, const QString& labelText,
												  IValidationStrategy* validationStrategy, bool isPassword)
{
	QLabel*				 label = new QLabel(labelText, this);
	QtMaterialTextField* field = new QtMaterialTextField(this);
	QLabel*				 errorLabel = new QLabel(this);
	errorLabel->setStyleSheet("QLabel { color: red; }");
	errorLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	errorLabel->setVisible(false);

	layout->addWidget(label);
	layout->addWidget(field);
	layout->addWidget(errorLabel);

	if (isPassword)
	{
		field->setEchoMode(QLineEdit::Password);
	}

	if (validationStrategy)
	{
		validationStrategies_.insert(field, validationStrategy);
		errorLabels_.insert(field, errorLabel);
	}

	return field;
}

void BaseDialog::validateInput(QtMaterialTextField* field)
{
	IValidationStrategy* strategy = validationStrategies_.value(field);
	QLabel*				 errorLabel = errorLabels_.value(field);
	bool				 isValid = strategy->isValid(field->text());

	bool isFieldEmpty = field->text().isEmpty();

	if (isFieldEmpty)
	{
		setFieldValidityStyle(field, errorLabel, "Field cannot be empty", false);
	}
	else
	{
		setFieldValidityStyle(field, errorLabel, strategy->errorMessage(), isValid);
	}

	updateConfirmButtonState();
}

void BaseDialog::updateConfirmButtonState()
{
	bool formIsValid = true;
	for (auto field: validationStrategies_.keys())
	{
		IValidationStrategy* strategy = validationStrategies_.value(field);
		formIsValid &= strategy->isValid(field->text()) && !field->text().isEmpty();
	}
	confirmButton->setEnabled(formIsValid);
}

void BaseDialog::setFieldValidityStyle(QtMaterialTextField* field, QLabel* errorLabel, const QString& errorMessage,
									   bool isValid)
{
	if (isValid)
	{
		field->setInputLineColor(Qt::green);
		errorLabel->setVisible(false);
	}
	else
	{
		field->setInputLineColor(Qt::red);
		errorLabel->setText(errorMessage);
		errorLabel->setVisible(true);
	}
}