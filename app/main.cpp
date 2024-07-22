#include <QApplication>
#include <QFontDatabase>
#include <QFont>
#include <QIcon>
#include "AppManager.h" // Include your AppManager header

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	// Load custom fonts from resources
	QFontDatabase fontDatabase;
	int			  fontIdRegular = fontDatabase.addApplicationFont(":/fonts/FiraSans-Regular.ttf");
	int			  fontIdBold = fontDatabase.addApplicationFont(":/fonts/FiraSans-Bold.ttf");
	int			  fontIdItalic = fontDatabase.addApplicationFont(":/fonts/FiraSans-Italic.ttf");
	int			  fontIdUltra = fontDatabase.addApplicationFont(":/fonts/FiraSans-Ultra.ttf");

	// Set default font for the application
	QString fontFamily = "Fira Sans"; // Ensure this matches the actual family name
	QFont	font(fontFamily);
	a.setFont(font);

	QIcon icon = QIcon(":/icons/bank.png");

	// Set the application icon
	a.setWindowIcon(icon);

	// Create an instance of the main application manager
	AppManager& appManager = AppManager::getInstance();

	// Start the application
	appManager.start();

	return a.exec();
}
