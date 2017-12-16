#ifndef CRYPTO_HPP
#define CRYPTO_HPP

#include <QDebug>

#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QString>
#include <QVBoxLayout>

#include <array>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <thread>

#include "encrypt.hpp"
#include "decrypt.hpp"

class Crypto : public QWidget {
	Q_OBJECT
public:

	Crypto();

private slots:

	//GUI SIGNAL SLOTS

	void keyinput_slot();
	void keybrowse_slot();

	void textinput_slot();
	void textbrowse_slot();

	void outputpath_slot();
	void outputbrowse_slot();

	void startdencrypt_slot();
	void startencrypt_slot();

private:

	//HELPER FUNCTIONS

	//checks the validity of a certain file extension
	int checkExt(const std::string &filename, const std::string &expected_ext);

	//converts a string character to a unsigned byte, or char, with an MSB bool to determine if it's the most/least significant byte
	unsigned char string_2_byte(char & c, bool msb);


	//GUI CREATION FUNCTIONS

	//creates the key input pane of the GUI
	void create_key_input();

	QGroupBox * keyinputbox;
	QLineEdit * keyinputpath;
	QPushButton * keyinputbrowse;

	//creates the text input pane of the GUI
	void create_text_input();

	QGroupBox * textinputbox;
	QLineEdit * textinputpath;
	QPushButton * textinputbrowse;


	//creates the key size options pane of the GUI
	void create_key_options();

	QGroupBox * keyoptionsbox;
	QRadioButton * key128;
	QRadioButton * key192;
	QRadioButton * key256;

	//creates the encryption options pane of the GUI
	void create_encryption_options();

	QGroupBox * encryptionoptionsbox;
	QRadioButton * ECB;
	QRadioButton * CBC;


	//creates the output pane of the GUI
	void create_output();

	QGroupBox * outputbox;
	QLineEdit * outputpath;
	QPushButton * outputbrowse;

	//creates the final command pane of the GUI
	void create_command();

	QGroupBox * commandbox;
	QPushButton * startdencrypt;
	QPushButton * startencrypt;

	//PRIVATE MEMBERS

	//key read from the key file
	std::vector<unsigned char> key;

	//4x4 blocks of text read in from the text file
	std::vector<std::array<unsigned char, 16>> input_arrays;

};


#endif
