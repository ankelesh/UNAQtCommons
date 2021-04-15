#include "BarcodeObserver.h"
#include <qapplication.h>
#include <settings/CommonAppSettings.h>
#ifdef DEBUG
#include <debug/debugtrace.h>
#endif
#ifdef Q_OS_ANDROID
#include <wrappers/Platform/Android/QBroadcastCatcher.h>
#endif



QString _initiateSequence(QChar ch)
{
    return QString(ch);
}


bool BarcodeObserver::eventFilter(QObject* object, QEvent* event)
{
	switch (event->type())
	{
	case QEvent::KeyPress:
	{
		QKeyEvent* temp = static_cast<QKeyEvent*>(event);
		if (active)
			if (_testPressureForBarcode(temp, object))
				return true;
		if (_testGlobalHotkeyPress(temp, object))
			return true;
	}
	break;
	case (QEvent::KeyRelease):
	{
		if (active)
			if (_testReleaseForBarcode(event))
				return true;
		QKeyEvent* temp = static_cast<QKeyEvent*>(event);
		if (_testGlobalHotkeyRelease(temp, object))
			return true;
		if (numberCatchingActive)
			_testNumberReleased(temp);
	}
	break;
	default:
		break;
	}
	return QObject::eventFilter(object, event);
	
}

bool BarcodeObserver::_testPressureForBarcode(QKeyEvent * ev, QObject* /*target*/)
{
	if (prefixFound)
	{
        QString etext = ev->text();
        if (ev->text() == suffix)
		{

				prefixFound = false;
                _handleBarcodeCatch(buffer);
				emit suffixCaught();
				ev->accept();
				buffer.clear();
				return true;
			}
			else
			{
				buffer += ev->text();
				ev->accept();
				return true;
			}
	}
	else
    {
        QString etext = ev->text();
            if (ev->text() == prefix)
			{
				prefixFound = true;
				ev->accept();
				emit prefixCaught();
				return true;
			}

	}
	return false;
}

bool BarcodeObserver::_testReleaseForBarcode(QEvent* ev)
{
	if (prefixFound)
	{
		ev->accept();
		return true;
	}
	else
		return false;
}

bool BarcodeObserver::_testGlobalHotkeyPress(QKeyEvent* ev, QObject* /*target*/)
{
	switch (ev->key())
	{

	default:
		return false;
	}
}

bool BarcodeObserver::_testGlobalHotkeyRelease(QKeyEvent* ev, QObject* /*target*/)
{
	switch (ev->key())
	{
	case Qt::Key_Escape:
	case Qt::Key_Back:
		emit escapeCaught();
		ev->accept();
		return true;
	case Qt::Key_Left:
	case Qt::Key_Right:
	case Qt::Key_Up:
	case Qt::Key_Down:
		emit arrowCaught(ev->key());
		return true;
	case Qt::Key_Return:
		emit returnCaught();
		return false;
	default:
		return false;
	}
}

bool BarcodeObserver::_testNumberReleased(QKeyEvent* ev)
{
	switch (ev->key())
	{
	case Qt::Key_0:
		emit numberPressed(0);
		break;
	case Qt::Key_1:
        emit numberPressed(1);
		break;
	case Qt::Key_2:
        emit numberPressed(2);
		break;
	case Qt::Key_3:
        emit numberPressed(3);
		break;
	case Qt::Key_4:
        emit numberPressed(4);
		break;
	case Qt::Key_5:
        emit numberPressed(5);
		break;
	case Qt::Key_6:
        emit numberPressed(6);
		break;
	case Qt::Key_7:
        emit numberPressed(7);
		break;
	case Qt::Key_8:
        emit numberPressed(8);
		break;
	case Qt::Key_9:
        emit numberPressed(9);
		break;
	default:
		return false;
	}
	return false;
}

	BarcodeObserver::BarcodeObserver(QChar pref, QChar suff, QObject* parent)
	: QObject(parent), prefix(_initiateSequence(pref)), 
	suffix(_initiateSequence(suff)), buffer(), prefixFound(false), active(false)
{
#ifdef Q_OS_ANDROID
    // datalogic defaults
    QBroadcastCatcher::listenForBroadcast("com.datalogic.decodewedge.decode_action",
                                          "com.datalogic.decode.intentwedge.barcode_string",
                                          "com.datalogic.decodewedge.decode_category" );
    // newland defaults
    QBroadcastCatcher::listenForBroadcast("nlscan.action.SCANNER_RESULT",
                                          "SCAN_BARCODE1",
                                          "" );
   QObject::connect(QBroadcastCatcher::instanse(), &QBroadcastCatcher::intentCaptured, this, &BarcodeObserver::handleIntentBarcode);
#endif
}

void BarcodeObserver::resetCapture(QChar pref, QChar suff)
{
	prefix = _initiateSequence(pref);
	suffix = _initiateSequence(suff);
	buffer.clear();
	prefixFound = false;
}

void BarcodeObserver::activate()
{
	active = true;
}

void BarcodeObserver::deactivate()
{
	active = false;
}
void BarcodeObserver::catchNumbers()
{
	++numberCatchingActive;
}
void BarcodeObserver::stopNumberCatching()
{
	--numberCatchingActive;
}
bool BarcodeObserver::activated()
{
	return active;
}
BarcodeObserver* BarcodeObserver::_instanse = Q_NULLPTR;
BarcodeObserver* BarcodeObserver::instanse()
{
	if (_instanse == Q_NULLPTR)
	{
		_instanse = new BarcodeObserver(QChar(CommonSettings->scanPrefix), QChar(CommonSettings->scanSuffix) , Q_NULLPTR);
	}
	return _instanse;
}

void BarcodeObserver::init()
{
	qApp->installEventFilter(instanse());
}

void BarcodeObserver::handleIntentBarcode(uint /*nhash*/, QString iname, QString barcode)
{
#ifdef DEBUG
	detrace_METHEXPL("obtained barcode by intent " << iname << " : " << barcode);
#endif
	_handleBarcodeCatch(barcode);
}

void BarcodeObserver::_handleBarcodeCatch(QString bc)
{
	if (active)
	{
		emit barcodeCaught(bc);
	}
}
