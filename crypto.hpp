#ifndef MIDIPLAYER_HPP
#define MIDIPLAYER_HPP

#include <QDebug>

#include <QAudioOutput>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSlider>
#include <QString>
#include <QVBoxLayout>

#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include "message_queue.hpp"
#include "message_queue.cpp"

#include "instrument.hpp"
#include "midi.hpp"
#include "signal.hpp"
#include "track.hpp"
#include "shared_double_buffer.hpp"
#include "shared_double_buffer.cpp"
#include "synthesizer.hpp"

enum MESSAGE_CODE { READ_FILE, PLAY, PAUSE, STOP};

class MIDIPlayer : public QWidget {
	Q_OBJECT

public:

	MIDIPlayer();

private slots:

	void inputbrowse_slot();

	void play_slot();
	void inputpath_slot();
	void requestaudio_slot();

	void pause_slot();
	void stop_slot();
	void volume_slot();
	void mute_slot();

private:

	QGroupBox * inputbox;
	QLineEdit * inputpath;
	QString * inputstring;
	QPushButton * inputbrowse;

	QGroupBox * audiocontrols;
	QPushButton * play;
	QPushButton * pause;
	QPushButton * stop;
	QLabel * volume_label;
	QSlider * volume;
	QPushButton * mute;

	message_queue<std::pair<int, std::string>> channel;
	shared_double_buffer<double> audio_buffer;

	bool init_play;
	bool paused;

	TrackListType midi_tracks;
	Track default_track;
	DefaultInstrument instrument;

	std::size_t buffer_size;
	std::vector<char> buffer;

	QAudioOutput *audio;
	QIODevice *device;

	void createInput();
	void createVolumeControls();

	int checkExt(const std::string &filename, const std::string &expected_ext);

	void synthesizer();

};

  
#endif
