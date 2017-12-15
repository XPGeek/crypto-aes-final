#include "crypto.hpp"

Crypto::Crypto()
{
	create_input();
	create_key_options();
	create_encryption_options();
	create_output();
	create_command();

	QGridLayout * layout = new QGridLayout;
	layout->addWidget(inputbox, 0, 0, 1, 2);
	layout->addWidget(keyoptionsbox, 1, 0);
	layout->addWidget(encryptionoptionsbox, 1, 1);
	layout->addWidget(outputbox, 2, 0, 1, 2);
	layout->addWidget(commandbox, 3, 0, 1, 2);

	setLayout(layout);

	setWindowTitle(QString("Encryptor v0.1"));
}

void Crypto::inputpath_slot() {

	if (!inputpath->text().isEmpty() && !inputpath->text().isNull()) {

		std::string in_file = inputpath->text().toStdString();

		if (checkExt(in_file, ".txt")) { //INVALID INPUT FILE EXTENSION
			QMessageBox input_ext_error;
			input_ext_error.setText("The File Input had an incorrect extension.");
			input_ext_error.setWindowTitle("Error - Input Extension");
			input_ext_error.exec();

			return;
		}

		std::ifstream input_file(in_file, std::ios::binary);

		if (!input_file.good()) { //BAD INPUT FILE
			QMessageBox input_file_error;
			input_file_error.setText("The input file did not exist, or could not be opened.");
			input_file_error.setWindowTitle("Error - Input File Invalid");
			input_file_error.exec();

			return;
		}

		//READ INTO ARRAY

		char c;

		while (input_file.get(c))
		{
			switch(c)
			{
				case '0':

				break;
			
				case '1':

				break;

				case '2'
			}
			input_vector.push_back(c);
			std::cout << c << std::endl;
		}

		input_file.close();

	} else {
		QMessageBox no_input_file;
		no_input_file.setText("Please enter a MIDI input file name.");
		no_input_file.setWindowTitle("Error - No MIDI Input File");
		no_input_file.exec();

		return;
	}
}

void Crypto::inputbrowse_slot()
{
	/*
	QString inputstring = QFileDialog::getOpenFileName(this, tr("Open MIDI Input"), "", tr("MIDI Files (*.mid *.midi)"));
	inputpath->setText(inputstring);
	*/
}

void Crypto::key128_slot()
{
}

void Crypto::key192_slot()
{
}

void Crypto::key256_slot()
{
}

void Crypto::ECB_slot()
{
}

void Crypto::CBC_slot()
{
}

void Crypto::outputpath_slot()
{
}

void Crypto::outputbrowse_slot()
{
}

void Crypto::startdencrypt_slot()
{
}

void Crypto::startencrypt_slot()
{
}

//HELPER FUNCTIONS

int Crypto::checkExt(const std::string &filename, const std::string &expected_ext) {

	if (filename.find_last_of(".") == std::string::npos) { //no file extension
		return 1;
	}

	std::string extension = filename.substr(filename.find_last_of("."), filename.length()); //gets extension

	if (extension != expected_ext) { //the file had an extension, but it was incorrect
		return 2;
	}
	else if (filename.length() == extension.length()) { //no filename
		return 3;
	}

	return 0;
}

//GUI CREATION FUNCTIONS

void Crypto::create_input()
{
	inputbox = new QGroupBox(tr("Input:"));
	inputbox->setObjectName("inputbox");

	QGridLayout * layout = new QGridLayout;

	inputpath = new QLineEdit();
	inputpath->setObjectName("inputpath");
	connect(inputpath, SIGNAL(textChanged(QString)), this, SLOT(inputpath_slot()));
	layout->addWidget(inputpath, 0, 0);

	inputbrowse = new QPushButton(tr("Browse"));
	layout->addWidget(inputbrowse, 0, 1);
	connect(inputbrowse, SIGNAL(clicked()), this, SLOT(inputbrowse_slot()));

	inputbox->setLayout(layout);
}

void Crypto::create_key_options()
{
	keyoptionsbox = new QGroupBox(tr("Key Size:"));
	QGridLayout * layout = new QGridLayout;

	key128 = new QRadioButton(tr("128-bit"));
	layout->addWidget(key128, 0, 0);
	connect(key128, SIGNAL(clicked()), this, SLOT(key128_slot()));

	key192 = new QRadioButton(tr("192-bit"));
	layout->addWidget(key192, 1, 0);
	connect(key192, SIGNAL(clicked()), this, SLOT(key192_slot()));
	
	key256 = new QRadioButton(tr("256-bit"));
	layout->addWidget(key256, 2, 0);
	connect(key256, SIGNAL(clicked()), this, SLOT(key256_slot()));

	keyoptionsbox->setLayout(layout);
}

void Crypto::create_encryption_options()
{
	encryptionoptionsbox = new QGroupBox(tr("Encryption Mode:"));
	QGridLayout * layout = new QGridLayout;

	ECB = new QRadioButton(tr("ECB Mode"));
	layout->addWidget(ECB, 0, 0);
	connect(ECB, SIGNAL(clicked()), this, SLOT(ECB_slot()));

	CBC = new QRadioButton(tr("CBC Mode"));
	layout->addWidget(CBC, 1, 0);
	connect(CBC, SIGNAL(clicked()), this, SLOT(CBC_slot()));

	encryptionoptionsbox->setLayout(layout);
}

void Crypto::create_output()
{
	outputbox = new QGroupBox(tr("Output:"));
	outputbox->setObjectName("outputbox");

	QGridLayout * layout = new QGridLayout;

	outputpath = new QLineEdit();
	outputpath->setObjectName("outputpath");
	connect(outputpath, SIGNAL(textChanged(QString)), this, SLOT(outputpath_slot()));
	layout->addWidget(outputpath, 0, 0);

	outputbrowse = new QPushButton(tr("Browse"));
	layout->addWidget(outputbrowse, 0, 1);
	connect(outputbrowse, SIGNAL(clicked()), this, SLOT(outputbrowse_slot()));

	outputbox->setLayout(layout);
}

void Crypto::create_command()
{
	commandbox = new QGroupBox();
	outputbox->setObjectName("commandbox");

	QGridLayout * layout = new QGridLayout;

	startdencrypt = new QPushButton(tr("Decrypt"));
	layout->addWidget(startdencrypt, 1, 0);
	connect(startdencrypt, SIGNAL(clicked()), this, SLOT(startdencrypt_slot()));

	startencrypt = new QPushButton(tr("Ecrypt"));
	layout->addWidget(startencrypt, 1, 1);
	connect(startencrypt, SIGNAL(clicked()), this, SLOT(startencrypt_slot()));

	commandbox->setLayout(layout);
}

