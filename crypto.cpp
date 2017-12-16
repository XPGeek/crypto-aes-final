#include "crypto.hpp"

Crypto::Crypto()
{
	create_key_input();
	create_text_input();
	create_key_options();
	create_encryption_options();
	create_output();
	create_command();

	QGridLayout * layout = new QGridLayout;
	layout->addWidget(keyinputbox, 0, 0, 1, 2);
	layout->addWidget(textinputbox, 1, 0, 1, 2);
	layout->addWidget(keyoptionsbox, 2, 0);
	layout->addWidget(encryptionoptionsbox, 2, 1);
	layout->addWidget(outputbox, 3, 0, 1, 2);
	layout->addWidget(commandbox, 4, 0, 1, 2);

	setLayout(layout);

	setWindowTitle(QString("Encryptor v0.9"));
}

void Crypto::keyinput_slot()
{
	if (!keyinputpath->text().isEmpty() && !keyinputpath->text().isNull()) {

		std::string in_file = keyinputpath->text().toStdString();

		if (checkExt(in_file, ".txt")) { //INVALID INPUT FILE EXTENSION
			QMessageBox input_ext_error;
			input_ext_error.setText("The key file has an incorrect extension.");
			input_ext_error.setWindowTitle("Error - File Extension");
			input_ext_error.exec();

			return;
		}

		std::ifstream input_file(in_file, std::ios::binary);

		if (!input_file.good()) { //BAD INPUT FILE
			QMessageBox input_file_error;
			input_file_error.setText("The key file could not be opened.");
			input_file_error.setWindowTitle("Error - File Issue");
			input_file_error.exec();

			return;
		}

		//READ INTO ARRAY

		key.clear();

		char c;

		unsigned char current_char;

		while (input_file.get(c))
		{
			current_char = 0;

			//MSB
			current_char += string_2_byte(c, true);

			//next character
			input_file.get(c);

			//MSB
			current_char += string_2_byte(c, false);
				
			//Put it into the vector
			key.push_back(current_char);

			std::cout << current_char << std::endl;
		}

		input_file.close();

	} else {
		QMessageBox no_input_file;
		no_input_file.setText("Please enter a key filename.");
		no_input_file.setWindowTitle("Error - No File");
		no_input_file.exec();

		return;
	}
}

void Crypto::keybrowse_slot()
{
	QString inputstring = QFileDialog::getOpenFileName(this, tr("Open Key"), "", tr("Text Files (*.txt)"));
	keyinputpath->setText(inputstring);
}

void Crypto::textinput_slot()
{
	if (!textinputpath->text().isEmpty() && !textinputpath->text().isNull()) {

		std::string in_file = textinputpath->text().toStdString();

		if (checkExt(in_file, ".txt")) { //INVALID INPUT FILE EXTENSION
			QMessageBox input_ext_error;
			input_ext_error.setText("The text file has an incorrect extension.");
			input_ext_error.setWindowTitle("Error - File Extension");
			input_ext_error.exec();

			return;
		}

		std::ifstream input_file(in_file, std::ios::binary);

		if (!input_file.good()) { //BAD INPUT FILE
			QMessageBox input_file_error;
			input_file_error.setText("The text file could not be opened.");
			input_file_error.setWindowTitle("Error - File Inaccessible");
			input_file_error.exec();

			return;
		}

		input_arrays.clear();

		//READ INTO ARRAY

		char c;

		std::array<unsigned char, 16> current_array;
		unsigned char current_char;

		int index = 0;

		while (input_file.get(c))
		{
			if (index == 16) {
				input_arrays.push_back(current_array);
				index = 0;
			}

			current_char = 0;

			//MSB
			current_char += string_2_byte(c, true);

			//next character
			input_file.get(c);

			//MSB
			current_char += string_2_byte(c, false);

			//Put it into the array
			current_array[index] = current_char;

			++index;

		}

		while (index != 16) {
			current_array[index] = 0x00;
			++index;
		}

		input_arrays.push_back(current_array);

		input_file.close();
		
	}
	else {
		QMessageBox no_input_file;
		no_input_file.setText("Please enter a text filename.");
		no_input_file.setWindowTitle("Error - No File");
		no_input_file.exec();

		return;
	}
}

void Crypto::textbrowse_slot()
{
	QString inputstring = QFileDialog::getOpenFileName(this, tr("Open Text"), "", tr("Text Files (*.txt)"));
	textinputpath->setText(inputstring);
}

void Crypto::outputpath_slot()
{
	if (!outputpath->text().isEmpty() && !outputpath->text().isNull()) {

		std::string out_file = outputpath->text().toStdString();

		if (checkExt(out_file, ".txt")) { //INVALID INPUT FILE EXTENSION
			QMessageBox input_ext_error;
			input_ext_error.setText("The output file has an incorrect extension.");
			input_ext_error.setWindowTitle("Error - File Extension");
			input_ext_error.exec();

			return;
		}

		std::ofstream output_file(out_file, std::ios::binary);

		if (!output_file.good()) { //BAD OUTPUT FILE
			QMessageBox input_file_error;
			input_file_error.setText("The output file could not be opened.");
			input_file_error.setWindowTitle("Error - File Issue");
			input_file_error.exec();

			return;
		}

		output_file.close();

	} else {
		QMessageBox no_output_file;
		no_output_file.setText("Please enter an output filename.");
		no_output_file.setWindowTitle("Error - No File");
		no_output_file.exec();

		return;
	}
}

void Crypto::outputbrowse_slot()
{
	QString outputstring = QFileDialog::getOpenFileName(this, tr("Save Output"), "", tr("Text Files (*.txt)"));
	outputpath->setText(outputstring);
}

void Crypto::startdencrypt_slot()
{

	outputpath_slot();

	if (key128->isChecked() && key.size() == 16 || key192->isChecked() && key.size() == 24 || key256->isChecked() && key.size() == 32) {
		
		int words = (int) key.size() / 4;
		int rounds = words + 6;
		
		if (ECB->isChecked()) {

			decrypt decryptor(rounds, words, false);

			decryptor.read_key(key);
			decryptor.key_expansion();

			std::string out_file = outputpath->text().toStdString();

			std::ofstream output_file(out_file);

			std::array<unsigned char, 16> current_output;

			for (int index = 0; index < input_arrays.size(); ++index)
			{
				current_output = decryptor.decrypt_block(input_arrays.at(index));

				for (int index = 0; index < current_output.size(); ++index)
				{
					output_file << std::hex << (int)current_output[index];
				}
			}

			output_file.close();

			return;

		}
		else if (CBC->isChecked()) {

			decrypt decryptor(rounds, words, true);

			decryptor.read_key(key);

			decryptor.key_expansion();

			std::array<unsigned char, 16> current_output;

			std::cout << std::endl << std::endl;


			for (int index = 0; index < input_arrays.size(); ++index)
			{
				current_output = decryptor.decrypt_block(input_arrays.at(index));

				for (int index = 0; index < current_output.size(); ++index)
				{
					std::cout << std::hex << (int)current_output[index] << " ";
				}

				std::cout << std::endl << std::endl;
			}

		}
	}
	else {
		QMessageBox no_options_selected;
		no_options_selected.setText("Please check your encryption options.\nThere were either no options selected,\nor the key provided did not match the size selected.");
		no_options_selected.setWindowTitle("Error - Encryption Options");
		no_options_selected.exec();

		return;
	}
}

void Crypto::startencrypt_slot()
{
	if (key128->isChecked() && key.size() == 16 || key192->isChecked() && key.size() == 24 || key256->isChecked() && key.size() == 32) {

		int words = key.size() / 4;
		int rounds = words + 6;

		if (ECB->isChecked()) {

			encrypt encryptor(rounds, words, false);

			encryptor.read_key(key);
			encryptor.key_expansion();

			std::array<unsigned char, 16> current_output;

			std::cout << std::endl << std::endl;

			for (int index = 0; index < input_arrays.size(); ++index)
			{
				current_output = encryptor.encrypt_block(input_arrays.at(index));

				for (int index = 0; index < current_output.size(); ++index)
				{
					std::cout << std::hex <<(int)current_output[index] << " ";
				}

				std::cout << std::endl << std::endl;
			}
		}
		else if (CBC->isChecked()) {
			std::cout << "bong" << std::endl;

			encrypt encryptor(rounds, words, true);

			encryptor.read_key(key);

			encryptor.key_expansion();

			std::array<unsigned char, 16> current_output;

			std::cout << std::endl << std::endl;


			for (int index = 0; index < input_arrays.size(); ++index)
			{
				current_output = encryptor.encrypt_block(input_arrays.at(index));

				for (int index = 0; index < current_output.size(); ++index)
				{
					std::cout << std::hex << (int)current_output[index] << " ";
				}

				std::cout << std::endl << std::endl;
			}
		}
	}
	else {
		QMessageBox no_options_selected;
		no_options_selected.setText("Please check your encryption options. There were either no options selected, or the key provided did not match the size selected.");
		no_options_selected.setWindowTitle("Error - Encryption Options");
		no_options_selected.exec();

		return;
	}
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


unsigned char Crypto::string_2_byte(char & c, bool msb)
{
	if (msb) {
		if (c == '0') {
			return 0x00;
		}
		else if (c == '1') {
			return 0x10;
		}
		else if (c == '2') {
			return 0x20;
		}
		else if (c == '3') {
			return 0x30;
		}
		else if (c == '4') {
			return 0x40;
		}
		else if (c == '5') {
			return 0x50;
		}
		else if (c == '6') {
			return 0x60;
		}
		else if (c == '7') {
			return 0x70;
		}
		else if (c == '8') {
			return 0x80;
		}
		else if (c == '9') {
			return 0x90;
		}
		else if (c == 'a' || c == 'A') {
			return 0xA0;
		}
		else if (c == 'b' || c == 'B') {
			return 0xB0;
		}
		else if (c == 'c' || c == 'C') {
			return 0xC0;
		}
		else if (c == 'd' || c == 'D') {
			return 0xD0;
		}
		else if (c == 'e' || c == 'E') {
			return 0xE0;
		}
		else if (c == 'f' || c == 'F') {
			return 0xF0;
		}
	}
	else {
		if (c == '0') {
			return 0x00;
		}
		else if (c == '1') {
			return 0x01;
		}
		else if (c == '2') {
			return 0x02;
		}
		else if (c == '3') {
			return 0x03;
		}
		else if (c == '4') {
			return 0x04;
		}
		else if (c == '5') {
			return 0x05;
		}
		else if (c == '6') {
			return 0x06;
		}
		else if (c == '7') {
			return 0x07;
		}
		else if (c == '8') {
			return 0x08;
		}
		else if (c == '9') {
			return 0x09;
		}
		else if (c == 'a' || c == 'A') {
			return 0x0A;
		}
		else if (c == 'b' || c == 'B') {
			return 0x0B;
		}
		else if (c == 'c' || c == 'C') {
			return 0x0C;
		}
		else if (c == 'd' || c == 'D') {
			return 0x0D;
		}
		else if (c == 'e' || c == 'E') {
			return 0x0E;
		}
		else if (c == 'f' || c == 'F') {
			return 0x0F;
		}
	}

	return 0x00;
}

//GUI CREATION FUNCTIONS

void Crypto::create_key_input()
{

	keyinputbox = new QGroupBox(tr("Key File:"));

	QGridLayout * layout = new QGridLayout;

	keyinputpath = new QLineEdit();
	connect(keyinputpath, SIGNAL(textChanged(QString)), this, SLOT(keyinput_slot()));
	layout->addWidget(keyinputpath, 0, 0);

	keyinputbrowse = new QPushButton(tr("Browse"));
	layout->addWidget(keyinputbrowse, 0, 1);
	connect(keyinputbrowse, SIGNAL(clicked()), this, SLOT(keybrowse_slot()));

	keyinputbox->setLayout(layout);

}

void Crypto::create_text_input()
{

	textinputbox = new QGroupBox(tr("Text File:"));

	QGridLayout * layout = new QGridLayout;

	textinputpath = new QLineEdit();
	connect(textinputpath, SIGNAL(textChanged(QString)), this, SLOT(textinput_slot()));
	layout->addWidget(textinputpath, 0, 0);

	textinputbrowse = new QPushButton(tr("Browse"));
	layout->addWidget(textinputbrowse, 0, 1);
	connect(textinputbrowse, SIGNAL(clicked()), this, SLOT(textbrowse_slot()));

	textinputbox->setLayout(layout);

}

void Crypto::create_key_options()
{
	keyoptionsbox = new QGroupBox(tr("Key Size:"));
	QGridLayout * layout = new QGridLayout;

	key128 = new QRadioButton(tr("128-bit"));
	layout->addWidget(key128, 0, 0);

	key192 = new QRadioButton(tr("192-bit"));
	layout->addWidget(key192, 1, 0);
	
	key256 = new QRadioButton(tr("256-bit"));
	layout->addWidget(key256, 2, 0);

	keyoptionsbox->setLayout(layout);
}

void Crypto::create_encryption_options()
{
	encryptionoptionsbox = new QGroupBox(tr("Encryption Mode:"));
	QGridLayout * layout = new QGridLayout;

	ECB = new QRadioButton(tr("ECB Mode"));
	layout->addWidget(ECB, 0, 0);

	CBC = new QRadioButton(tr("CBC Mode"));
	layout->addWidget(CBC, 1, 0);

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

