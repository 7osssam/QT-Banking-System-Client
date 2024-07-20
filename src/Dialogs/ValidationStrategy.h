/**
 * @file ValidationStrategy.h
 * @brief Header file for validation strategies.
 * @details Declares the interface and concrete classes for various validation strategies used in input validation.
**/

#ifndef VALIDATIONSTRATEGY_H
#define VALIDATIONSTRATEGY_H

#include <QRegularExpressionValidator>
#include <QString>

/**
 * @class IValidationStrategy
 * @brief Interface for validation strategies.
 * @details Provides a common interface for different validation strategies to validate input data.
 */
class IValidationStrategy
{
public:
	/**
	 * @brief Validates the input string.
	 * @param input The input string to validate.
	 * @return True if the input is valid, otherwise false.
	 */
	virtual bool isValid(const QString& input) const = 0;

	/**
	 * @brief Retrieves the error message for invalid input.
	 * @return The error message as a QString.
	 */
	virtual QString errorMessage() const = 0;
};

/**
 * @class NameValidationStrategy
 * @brief Concrete validation strategy for names.
 * @details Validates that the input string is a valid name consisting of only alphabetic characters.
 */
class NameValidationStrategy : public IValidationStrategy
{
public:
	/**
	 * @brief Validates the input string as a name.
	 * @param input The input string to validate.
	 * @return True if the input is a valid name, otherwise false.
	 */
	bool isValid(const QString& input) const override
	{
		const QRegularExpression namePattern(R"((^[A-Za-z]*$))");
		return namePattern.match(input).hasMatch();
	}

	/**
	 * @brief Retrieves the error message for an invalid name.
	 * @return The error message as a QString.
	 */
	QString errorMessage() const override
	{
		return "Invalid name format";
	}
};

/**
 * @class EmailValidationStrategy
 * @brief Concrete validation strategy for emails.
 * @details Validates that the input string is a valid email address.
 */
class EmailValidationStrategy : public IValidationStrategy
{
public:
	/**
	 * @brief Validates the input string as an email.
	 * @param input The input string to validate.
	 * @return True if the input is a valid email, otherwise false.
	 */
	bool isValid(const QString& input) const override
	{
		const QRegularExpression emailPattern(R"((^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,4}$))");
		return emailPattern.match(input).hasMatch();
	}

	/**
	 * @brief Retrieves the error message for an invalid email.
	 * @return The error message as a QString.
	 */
	QString errorMessage() const override
	{
		return "Invalid email format";
	}
};

/**
 * @class PasswordValidationStrategy
 * @brief Concrete validation strategy for passwords.
 * @details Validates that the input string is a valid password according to specified criteria.
 */
class PasswordValidationStrategy : public IValidationStrategy
{
public:
	/**
	 * @brief Validates the input string as a password.
	 * @param input The input string to validate.
	 * @return True if the input is a valid password, otherwise false.
	 */
	bool isValid(const QString& input) const override
	{
		const QRegularExpression passwordPattern(R"((?=.*\d)(?=.*[a-z])(?=.*[A-Z]).{8,})");
		return passwordPattern.match(input).hasMatch();
	}

	/**
	 * @brief Retrieves the error message for an invalid password.
	 * @return The error message as a QString.
	 */
	QString errorMessage() const override
	{
		return "Password must contain at least 8 characters, 1 digit, 1 lowercase, and 1 uppercase";
	}
};

/**
 * @class BalanceValidationStrategy
 * @brief Concrete validation strategy for balances.
 * @details Validates that the input string is a valid balance consisting of numeric characters.
 */
class BalanceValidationStrategy : public IValidationStrategy
{
public:
	/**
	 * @brief Validates the input string as a balance.
	 * @param input The input string to validate.
	 * @return True if the input is a valid balance, otherwise false.
	 */
	bool isValid(const QString& input) const override
	{
		const QRegularExpression balancePattern(R"((^[0-9]*$))");
		return balancePattern.match(input).hasMatch();
	}

	/**
	 * @brief Retrieves the error message for an invalid balance.
	 * @return The error message as a QString.
	 */
	QString errorMessage() const override
	{
		return "Invalid balance format";
	}
};

/**
 * @class AccountNumberValidationStrategy
 * @brief Concrete validation strategy for account numbers.
 * @details Validates that the input string is a valid account number consisting of exactly 10 digits.
 */
class AccountNumberValidationStrategy : public IValidationStrategy
{
public:
	/**
	 * @brief Validates the input string as an account number.
	 * @param input The input string to validate.
	 * @return True if the input is a valid account number, otherwise false.
	 */
	bool isValid(const QString& input) const override
	{
		const QRegularExpression accountNumberPattern(R"((^[0-9]{10}$))");
		return accountNumberPattern.match(input).hasMatch();
	}

	/**
	 * @brief Retrieves the error message for an invalid account number.
	 * @return The error message as a QString.
	 */
	QString errorMessage() const override
	{
		return "Account number must contain 10 digits";
	}
};

#endif // VALIDATIONSTRATEGY_H
