#include "midiplayer.hpp"

MIDIPlayer::MIDIPlayer() : audio_buffer(44100) {

	std::thread synth_thread(&MIDIPlayer::synthesizer, this);
	synth_thread.detach();

	createInput();
	createVolumeControls();

	volume_slot();

	QVBoxLayout * mainLayout = new QVBoxLayout;
	mainLayout->addWidget(inputbox);
	mainLayout->addWidget(audiocontrols);

	setLayout(mainLayout);


	init_play = true;
	paused = false;

}

void MIDIPlayer::inputbrowse_slot() {
	QString inputstring = QFileDialog::getOpenFileName(this, tr("Open MIDI Input"), "", tr("MIDI Files (*.mid *.midi)"));
	inputpath->setText(inputstring);
}

void MIDIPlayer::inputpath_slot() {
	if (!inputpath->text().isEmpty() && !inputpath->text().isNull()) {

		std::string in_file = inputpath->text().toStdString();

		if (checkExt(in_file, ".mid") && checkExt(in_file, ".midi")) { //INVALID INPUT FILE EXTENSION
			QMessageBox input_ext_error;
			input_ext_error.setText("The MIDI input file has an incorrect extension.");
			input_ext_error.setWindowTitle("Error - MIDI Input Extension");
			input_ext_error.exec();

			return;
		}

		std::ifstream input_file(in_file, std::ios::binary);

		if (!input_file.good()) { //BAD INPUT FILE
			QMessageBox input_file_error;
			input_file_error.setText("The MIDI input file did not exist, or could not be opened.");
			input_file_error.setWindowTitle("Error - MIDI Input File Invalid");
			input_file_error.exec();

			return;
		}

		input_file.close();
	}
	else {
		QMessageBox no_input_file;
		no_input_file.setText("Please enter a MIDI input file name.");
		no_input_file.setWindowTitle("Error - No MIDI Input File");
		no_input_file.exec();

		return;
	}

	//SEND READ FILE COMMAND

	std::string in_file = inputpath->text().toStdString();
	channel.push(std::pair <int, std::string>(READ_FILE, in_file));
}

void MIDIPlayer::requestaudio_slot() {

	int bytesFree = audio->bytesFree();
	int bytesToWrite = bytesFree > buffer_size ? buffer_size : bytesFree;
	int samplesToWrite = bytesToWrite >> 1;
	bytesToWrite = 2 * samplesToWrite;

	//qDebug() << "Bytes Free: " << bytesFree << " Bytes to Write: " << bytesToWrite << " SamplesToWrite: " << samplesToWrite;

	buffer.reserve(bytesToWrite);

	double new_sample;

	for (size_t i = 0; i < samplesToWrite; ++i) {
		
		int16_t temp = 0;
		
		if (!paused) {
			if (audio_buffer.read(new_sample)) {
				temp = static_cast<int16_t>(lround(new_sample));
			}
		}

		char *byte1 = reinterpret_cast<char *>(&temp);
		++byte1;
		char *byte2 = byte1;
		buffer.push_back(*byte1);
		buffer.push_back(*byte2);

	}

	if (bytesToWrite > 0) {
		device->write(buffer.data(), bytesToWrite);
	}
	
	buffer.clear();
}

void MIDIPlayer::play_slot() {

	if (midi_tracks.empty()) {
		QMessageBox no_input_file;
		no_input_file.setText("There is no data to play! Check your input file name!");
		no_input_file.setWindowTitle("Error - No MIDI Data Available");
		no_input_file.exec();

		return;
	}

	channel.push(std::pair <int, std::string>(PLAY, ""));
	paused = false;

	play->setEnabled(false);
	inputbrowse->setEnabled(false);
	inputpath->setEnabled(false);
}

void MIDIPlayer::pause_slot() {
	channel.push(std::pair <int, std::string>(PAUSE, ""));
	init_play = false;
	paused = true;

	play->setEnabled(true);
}

void MIDIPlayer::stop_slot() {
	channel.push(std::pair <int, std::string>(STOP, ""));
	audio_buffer.clear();
	init_play = true;
	paused = false;

	play->setEnabled(true);
	inputbrowse->setEnabled(true);
	inputpath->setEnabled(true);
}

void MIDIPlayer::volume_slot() {
	audio->setVolume(qreal((double)volume->value() / (double)volume->maximum()));
}

void MIDIPlayer::mute_slot() {
	volume->setValue(0);
}

void MIDIPlayer::createInput()
{
	inputbox = new QGroupBox(tr("MIDI Input"));
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

void MIDIPlayer::createVolumeControls()
{
	//CREATE GRID LAYOUT

	audiocontrols = new QGroupBox(tr("Audio Contols"));
	QGridLayout * layout = new QGridLayout;

	//CREATE BUTTONS

	play = new QPushButton(tr("Play"));
	play->setObjectName("play");
	layout->addWidget(play, 0, 0);
	connect(play, SIGNAL(clicked()), this, SLOT(play_slot()));

	pause = new QPushButton(tr("Pause"));
	pause->setObjectName("pause");
	layout->addWidget(pause, 0, 1);
	connect(pause, SIGNAL(clicked()), this, SLOT(pause_slot()));

	stop = new QPushButton(tr("Stop"));
	stop->setObjectName("stop");
	layout->addWidget(stop, 0, 2);
	connect(stop, SIGNAL(clicked()), this, SLOT(stop_slot()));

	//CREATE VOLUME LABEL

	volume_label = new QLabel(tr("Volume:"));
	layout->addWidget(volume_label, 1, 0);

	//INITIALIZE AUDIO DEVICE

	QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());

	auto rates = info.supportedSampleRates();

	int sampleRate = rates.at(0);
	for (int i = 1; i < rates.size(); ++i) {
		if (rates.at(i) < sampleRate)
			sampleRate = rates.at(i);
	}

	QAudioFormat format;
	format.setSampleRate(sampleRate);
	format.setChannelCount(1);
	format.setSampleSize(16);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::SignedInt);

	if (!info.isFormatSupported(format)) {
		qWarning() << "Raw audio format not supported by backend, cannot play audio.";
	} else {

		audio = new QAudioOutput(format);

		connect(audio, SIGNAL(notify()), this, SLOT(requestaudio_slot()));

		buffer_size = audio->bufferSize();
		buffer_size = buffer_size > 0 ? buffer_size : 1024;

		audio->setNotifyInterval(25);

	}

	device = audio->start();

	requestaudio_slot();

	//CREATE VOLUME SLIDER

	volume = new QSlider(Qt::Horizontal);
	volume->setObjectName("volume");
	volume->setMinimum(0);
	volume->setMaximum(10000);
	volume->setValue(5000);
	layout->addWidget(volume, 1, 1);
	connect(volume, SIGNAL(valueChanged(int)), this, SLOT(volume_slot()));

	mute = new QPushButton(tr("Mute"));
	mute->setObjectName("mute");
	layout->addWidget(mute, 1, 2);
	connect(mute, SIGNAL(clicked()), this, SLOT(mute_slot()));

	audiocontrols->setLayout(layout);
}

int MIDIPlayer::checkExt(const std::string &filename, const std::string &expected_ext) {

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

void MIDIPlayer::synthesizer() {

	bool init_play = true;

	int sample_rate;

	std::pair<int, std::string> msg;

	channel.wait_and_pop(msg);

	while (true) {
		
		if (msg.first == READ_FILE) {
			
			midi_tracks.clear();
			midi_tracks = readMIDIFromFile(msg.second);
			default_track = midi_tracks.front();
			
			instrument.reset(); //reset instrument
			instrument.load_track(default_track);

			QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());

			auto rates = info.supportedSampleRates();
			sample_rate = rates.at(0);
		
		} else if (msg.first == PLAY) {

			if (init_play) { //first time playing, or playing from a stop.
				instrument.reset(); //reset instrument
				instrument.load_track(default_track);
				audio_buffer.clear();
				buffer.clear();
			}

			while (channel.empty()) {
				if (!instrument.halted()) {
					audio_buffer.write(instrument.sample(sample_rate));
				}
				else {
					audio_buffer.write(0);
				}
			}

			channel.wait_and_pop(msg);

		}

		channel.wait_and_pop(msg);

	}
}
