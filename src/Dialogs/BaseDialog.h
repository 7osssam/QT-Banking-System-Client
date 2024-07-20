/**
 * @file BaseDialog.h
 * @brief Header file for the BaseDialog class.
 * @details Declares the BaseDialog class which provides a base implementation for dialog boxes with validation and UI setup.
**/

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

/**
 * @class BaseDialog
 * @brief The BaseDialog class provides a base class for creating dialogs with validation support.
 * @details This class is responsible for setting up the user interface, connecting signals, and validating input fields.
 */
class BaseDialog : public QDialog
{
	Q_OBJECT

public:
	/**
	 * @brief Constructor for BaseDialog.
	 * @param parent The parent widget.
	 */
	BaseDialog(QWidget* parent = nullptr);

	/**
	 * @brief Pure virtual function to get the data from the dialog.
	 * @return A QVariantMap containing the data.
	 */
	virtual QVariantMap getData() const = 0;

protected:
	/**
	 * @brief Pure virtual function to set up the user interface.
	 */
	virtual void setupUI() = 0;

	/**
	 * @brief Connects signals for the dialog.
	 */
	virtual void connectSignals();

	/**
	 * @brief Creates an input field with validation.
	 * @param layout The layout to add the input field to.
	 * @param labelText The text for the label associated with the input field.
	 * @param validationStrategy The validation strategy for the input field.
	 * @param isPassword Boolean flag to indicate if the input field is for password input.
	 * @return A pointer to the created input field.
	 */
	QtMaterialTextField* createInputField(QVBoxLayout* layout, const QString& labelText,
										  IValidationStrategy* validationStrategy, bool isPassword = false);

	/**
	 * @brief Validates the input of a specified field.
	 * @param field The input field to validate.
	 */
	void validateInput(QtMaterialTextField* field);

	/**
	 * @brief Updates the state of the confirm button based on the validity of the form.
	 */
	void updateConfirmButtonState();

	/**
	 * @brief Sets the validity style of an input field.
	 * @param field The input field to update.
	 * @param errorLabel The label to display error messages.
	 * @param errorMessage The error message to display.
	 * @param isValid Boolean flag to indicate if the input is valid.
	 */
	void setFieldValidityStyle(QtMaterialTextField* field, QLabel* errorLabel, const QString& errorMessage,
							   bool isValid);

	QMap<QtMaterialTextField*, IValidationStrategy*>
										validationStrategies_; ///< Stores validation strategies for input fields.
	QMap<QtMaterialTextField*, QLabel*> errorLabels_;		   ///< Stores error labels for input fields.
	QtMaterialFlatButton*				confirmButton;		   ///< The confirm button.
	QtMaterialFlatButton*				cancelButton;		   ///< The cancel button.
};

#endif														   // BASEDIALOG_H