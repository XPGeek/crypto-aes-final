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
#include <string>
#include <thread>

class Crypto : public QWidget {
	Q_OBJECT
public:

	Crypto();

private slots:

	void keyinput_slot();
	void keybrowse_slot();

	void textinput_slot();
	void textbrowse_slot();

	void key128_slot();
	void key192_slot();
	void key256_slot();

	void ECB_slot();
	void CBC_slot();

	void outputpath_slot();
	void outputbrowse_slot();

	void startdencrypt_slot();
	void startencrypt_slot();

private:

	//HELPER FUNCTIONS

	//checks the validity of a certain file extension
	int checkExt(const std::string &filename, const std::string &expected_ext);

	unsigned char char_2_char(char & c, bool msb);


	//GUI CREATION FUNCTIONS

	//creates the input pane of the GUI
	void create_key_input();

	QGroupBox * keyinputbox;
	QLineEdit * keyinputpath;
	QString * keyinputstring;
	QPushButton * keyinputbrowse;

	void create_text_input();

	QGroupBox * textinputbox;
	QLineEdit * textinputpath;
	QString * textinputstring;
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
	QString * outputstring;
	QPushButton * outputbrowse;

	void create_command();

	QGroupBox * commandbox;
	QPushButton * startdencrypt;
	QPushButton * startencrypt;

	//PRIVATE MEMBERS

	std::vector<unsigned char> key;
	std::vector<std::array<unsigned char, 16>> input_arrays;

};


#endif
