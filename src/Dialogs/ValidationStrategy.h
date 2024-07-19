#ifndef VALIDATIONSTRATEGY_H
#define VALIDATIONSTRATEGY_H

#include <QRegularExpressionValidator>
#include <QString>

// Validation Strategy Interface
class IValidationStrategy
{
public:
	virtual bool	isValid(const QString& input) const = 0;
	virtual QString errorMessage() const = 0;
};

// Concrete Validation Strategies
class NameValidationStrategy : public IValidationStrategy
{
public:
	bool isValid(const QString& input) const override
	{
		const QRegularExpression namePattern(R"((^[A-Za-z]*$))");
		return namePattern.match(input).hasMatch();
	}
	QString errorMessage() const override
	{
		return "Invalid name format";
	}
};

class EmailValidationStrategy : public IValidationStrategy
{
public:
	bool isValid(const QString& input) const override
	{
		const QRegularExpression emailPattern(R"((^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,4}$))");
		return emailPattern.match(input).hasMatch();
	}
	QString errorMessage() const override
	{
		return "Invalid email format";
	}
};

class PasswordValidationStrategy : public IValidationStrategy
{
public:
	bool isValid(const QString& input) const override
	{
		const QRegularExpression passwordPattern(R"((?=.*\d)(?=.*[a-z])(?=.*[A-Z]).{8,})");
		return passwordPattern.match(input).hasMatch();
	}
	QString errorMessage() const override
	{
		return "Password must contain at least 8 characters, 1 digit, 1 lowercase, 1 uppercase";
	}
};

class BalanceValidationStrategy : public IValidationStrategy
{
public:
	bool isValid(const QString& input) const override
	{
		const QRegularExpression balancePattern(R"((^[0-9]*$))");
		return balancePattern.match(input).hasMatch();
	}
	QString errorMessage() const override
	{
		return "Invalid balance format";
	}
};

class AccountNumberValidationStrategy : public IValidationStrategy
{
public:
	bool isValid(const QString& input) const override
	{
		const QRegularExpression accountNumberPattern(R"((^[0-9]{10}$))");
		return accountNumberPattern.match(input).hasMatch();
	}
	QString errorMessage() const override
	{
		return "Account number must contain 10 digits";
	}
};

#endif // VALIDATIONSTRATEGY_H
