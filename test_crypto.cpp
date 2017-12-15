#include <QTest>
#include <QPushButton>
#include <QSlider>
#include <QLineEdit>

#include "crypto.hpp"
#include "test_config.hpp"

class TestCrypto : public QObject {
	Q_OBJECT

	private slots:

	void initTestCase();

private:

	Crypto widget;

};

void TestCrypto::initTestCase() {

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



QTEST_MAIN(TestCrypto)
#include "test_crypto.moc"

