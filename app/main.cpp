#include <QApplication>
#include "AppManager.h" // Include your login widget header

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	// Create an instance of the main window
	AppManager& appManager = AppManager::getInstance();

	appManager.start();


	return a.exec();
}
