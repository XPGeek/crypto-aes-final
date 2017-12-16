#include <QApplication>

#include "crypto.hpp"

int main(int argc, char *argv[])
{
	//Constructs and draws the GUI for the application

	QApplication app(argc, argv);

	Crypto encryptor;

	encryptor.show();
	return app.exec();
}
