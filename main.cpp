#include <QApplication>

#include "crypto.hpp"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	Crypto encryptor;

	encryptor.show();
	return app.exec();
}
