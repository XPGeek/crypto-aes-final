#include <QTest>
#include <QPushButton>
#include <QSlider>
#include <QLineEdit>

#include "midiplayer.hpp"
#include "test_config.hpp"

class TestMIDIPlayer : public QObject {
  Q_OBJECT

private slots:

 void initTestCase();

 void PLAY_PAUSE_STOP_A();
 void PLAY_PAUSE_STOP_B();
 void PLAY_PAUSE_STOP_C();

 void VOLUME_CONTROL_A();
 void VOLUME_CONTROL_B();
  
private:

  MIDIPlayer widget;

};

void TestMIDIPlayer::initTestCase(){

  {
    auto w = widget.findChild<QLineEdit *>("inputpath");
    QVERIFY2(w, "Could not find a QLineEdit widget");
  }

  {
    auto w = widget.findChild<QPushButton *>("play");
    QVERIFY2(w, "Could not find play button");
  }

  {
    auto w = widget.findChild<QPushButton *>("pause");
    QVERIFY2(w, "Could not find pause button");
  }
  
  {
    auto w = widget.findChild<QPushButton *>("stop");
    QVERIFY2(w, "Could not find stop button");
  }

  {
    auto w = widget.findChild<QSlider *>("volume");
    QVERIFY2(w, "Could not find volume slider");
  }
  
  {
    auto w = widget.findChild<QPushButton *>("mute");
    QVERIFY2(w, "Could not find mute button");
  }
}

void TestMIDIPlayer::PLAY_PAUSE_STOP_A() {
	//Input file setup
	auto inputpath = widget.findChild<QLineEdit *>("inputpath");
	inputpath->setText(QString::fromStdString(TEST_FILE_DIR + "/test0.mid"));
	QTest::keyPress(inputpath, Qt::Key::Key_Return);
	
	//Find buttons
	auto pause = widget.findChild<QPushButton *>("pause");
	auto play = widget.findChild<QPushButton *>("play");
	auto stop = widget.findChild<QPushButton *>("stop");
	QTest::qWait(100);
	
	//Press Play, Pause, Play, Stop and then Play again.
	QTest::mouseClick(play, Qt::MouseButton::LeftButton);
	QCOMPARE(inputpath->isEnabled(), false);
	QCOMPARE(play->isEnabled(), false);
	QCOMPARE(pause->isEnabled(), true);
	QCOMPARE(stop->isEnabled(), true);
	QTest::qWait(500);
	QTest::mouseClick(pause, Qt::MouseButton::LeftButton);
	QTest::qWait(500);
	QTest::mouseClick(play, Qt::MouseButton::LeftButton);
	QTest::qWait(500);
	QTest::mouseClick(pause, Qt::MouseButton::LeftButton);
	QTest::qWait(100);
	QTest::mouseClick(stop, Qt::MouseButton::LeftButton);
	QTest::qWait(100);
	QTest::mouseClick(play, Qt::MouseButton::LeftButton);
	QTest::qWait(500);
	QTest::mouseClick(pause, Qt::MouseButton::LeftButton);
	QTest::qWait(500);
	QTest::mouseClick(play, Qt::MouseButton::LeftButton);
	QTest::qWait(500);
	QTest::mouseClick(stop, Qt::MouseButton::LeftButton);
	QTest::qWait(500);
	
}

void TestMIDIPlayer::PLAY_PAUSE_STOP_B() {
	//Input file setup
	auto inputpath = widget.findChild<QLineEdit *>("inputpath");
	inputpath->setText(QString::fromStdString(TEST_FILE_DIR + "/test5.mid"));
	QTest::keyPress(inputpath, Qt::Key::Key_Return);

	//Find buttons
	auto pause = widget.findChild<QPushButton *>("pause");
	auto play = widget.findChild<QPushButton *>("play");
	auto stop = widget.findChild<QPushButton *>("stop");
	QTest::qWait(100);

	//Press Play, Pause, Play, Stop and then Play again.
	QTest::mouseClick(play, Qt::MouseButton::LeftButton);
	QCOMPARE(inputpath->isEnabled(), false);
	QCOMPARE(play->isEnabled(), false);
	QCOMPARE(pause->isEnabled(), true);
	QCOMPARE(stop->isEnabled(), true);
	QTest::qWait(500);
	QTest::mouseClick(pause, Qt::MouseButton::LeftButton);
	QTest::qWait(500);
	QTest::mouseClick(play, Qt::MouseButton::LeftButton);
	QTest::qWait(500);
	QTest::mouseClick(pause, Qt::MouseButton::LeftButton);
	QTest::qWait(100);
	QTest::mouseClick(play, Qt::MouseButton::LeftButton);
	QTest::qWait(500);
	QTest::mouseClick(pause, Qt::MouseButton::LeftButton);
	QTest::qWait(100);
	QTest::mouseClick(play, Qt::MouseButton::LeftButton);
	QTest::qWait(500);
	QTest::mouseClick(pause, Qt::MouseButton::LeftButton);
	QTest::qWait(100);
	QTest::mouseClick(stop, Qt::MouseButton::LeftButton);
	QTest::qWait(100);
	QTest::mouseClick(play, Qt::MouseButton::LeftButton);
	QTest::qWait(500);
	QTest::mouseClick(pause, Qt::MouseButton::LeftButton);
	QTest::qWait(500);
	QTest::mouseClick(play, Qt::MouseButton::LeftButton);
	QTest::qWait(500);
	QTest::mouseClick(stop, Qt::MouseButton::LeftButton);
	QTest::qWait(500);
}

void TestMIDIPlayer::PLAY_PAUSE_STOP_C() {
	//Input file setup
	auto inputpath = widget.findChild<QLineEdit *>("inputpath");
	inputpath->setText(QString::fromStdString(TEST_FILE_DIR + "/test6.mid"));
	QTest::keyPress(inputpath, Qt::Key::Key_Return);

	//Find buttons
	auto pause = widget.findChild<QPushButton *>("pause");
	auto play = widget.findChild<QPushButton *>("play");
	auto stop = widget.findChild<QPushButton *>("stop");
	QTest::qWait(100);

	//Press Play, Pause, Play, Stop and then Play again.
	QTest::mouseClick(play, Qt::MouseButton::LeftButton);
	QCOMPARE(inputpath->isEnabled(), false);
	QCOMPARE(play->isEnabled(), false);
	QCOMPARE(pause->isEnabled(), true);
	QCOMPARE(stop->isEnabled(), true);
	QTest::qWait(500);
	QTest::mouseClick(pause, Qt::MouseButton::LeftButton);
	QTest::qWait(500);
	QTest::mouseClick(play, Qt::MouseButton::LeftButton);
	QTest::qWait(500);
	QTest::mouseClick(pause, Qt::MouseButton::LeftButton);
	QTest::qWait(100);
	QTest::mouseClick(play, Qt::MouseButton::LeftButton);
	QTest::qWait(500);
	QTest::mouseClick(pause, Qt::MouseButton::LeftButton);
	QTest::qWait(100);
	QTest::mouseClick(play, Qt::MouseButton::LeftButton);
	QTest::qWait(500);
	QTest::mouseClick(pause, Qt::MouseButton::LeftButton);
	QTest::qWait(100);
	QTest::mouseClick(stop, Qt::MouseButton::LeftButton);
	QTest::qWait(100);
	QTest::mouseClick(play, Qt::MouseButton::LeftButton);
	QTest::qWait(500);
	QTest::mouseClick(pause, Qt::MouseButton::LeftButton);
	QTest::qWait(500);
	QTest::mouseClick(play, Qt::MouseButton::LeftButton);
	QTest::qWait(500);
	QTest::mouseClick(stop, Qt::MouseButton::LeftButton);
	QTest::qWait(2000);

}

void TestMIDIPlayer::VOLUME_CONTROL_A() {
	//Input file setup
	auto inputpath = widget.findChild<QLineEdit *>("inputpath");
	inputpath->setText(QString::fromStdString(TEST_FILE_DIR + "/test0.mid"));
	QTest::keyPress(inputpath, Qt::Key::Key_Return);

	//Find buttons
	auto pause = widget.findChild<QPushButton *>("pause");
	auto play = widget.findChild<QPushButton *>("play");
	auto stop = widget.findChild<QPushButton *>("stop");
	
	auto volume = widget.findChild<QSlider *>("volume");
	auto mute = widget.findChild<QPushButton *>("mute");
	QTest::qWait(100);

	//Press Play, Pause, Play, Stop and then Play again.
	QTest::mouseClick(play, Qt::MouseButton::LeftButton);
	QTest::qWait(1000);
	volume->setValue(10000);
	QTest::qWait(1000);
	volume->setValue(1000);
	QTest::qWait(1000);
	volume->setValue(10000);
	QTest::qWait(1000);
	volume->setValue(5000);
	QTest::qWait(1000);

}

void TestMIDIPlayer::VOLUME_CONTROL_B() {
	//Input file setup
	auto inputpath = widget.findChild<QLineEdit *>("inputpath");
	inputpath->setText(QString::fromStdString(TEST_FILE_DIR + "/test0.mid"));
	QTest::keyPress(inputpath, Qt::Key::Key_Return);

	//Find buttons
	auto pause = widget.findChild<QPushButton *>("pause");
	auto play = widget.findChild<QPushButton *>("play");
	auto stop = widget.findChild<QPushButton *>("stop");

	auto volume = widget.findChild<QSlider *>("volume");
	auto mute = widget.findChild<QPushButton *>("mute");
	QTest::qWait(100);

	//Press Play, Pause, Play, Stop and then Play again.
	QTest::mouseClick(play, Qt::MouseButton::LeftButton);
	QTest::qWait(1000);
	volume->setValue(1000);
	QTest::qWait(1000);
	volume->setValue(3000);
	QTest::qWait(1000);
	volume->setValue(5000);
	QTest::qWait(1000);
	volume->setValue(7000);
	QTest::qWait(1000);
	mute->click();
	QTest::qWait(1000);

}



QTEST_MAIN(TestMIDIPlayer)
#include "test_midiplayer.moc"

